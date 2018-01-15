#include "small_obj.h"
#include <cassert>

using namespace Mcd;

void FixedAllocator::Chunk::Init(std::size_t blockSize, unsigned char blocks) {
    assert(blocks > 0 && blockSize > 0); 
    // check overflow
    assert((blocks * blockSize) / blockSize == blocks);
    pData_ = new unsigned char[blocks * blockSize];
    Reset(blockSize, blocks);
}

void FixedAllocator::Chunk::Reset(std::size_t blockSize, unsigned char blocks) {
    assert(blocks > 0 && blockSize > 0); 
    // check overflow
    assert((blocks * blockSize) / blockSize == blocks);

    firstAvailableBlock_ = 0;
    blocksAvailable_ = blocks;

    unsigned char i = 0;
    for (unsigned char *p = pData_; i != blocks; p += blockSize) {
        /*
            every blockSize bytes is seen as a block
            the first byte of a block is labeled of an index
            the index is starting from 1
        */
        *p = ++i;
    }
}

void FixedAllocator::Chunk::Release() {
    delete[] pData_;
}

void* FixedAllocator::Chunk::Allocate(std::size_t blockSize) {
    // all chunk has been used
    if (blocksAvailable_ == 0)
        return 0; 
    unsigned char* ret = pData_ + firstAvailableBlock_ * blockSize;
    --blocksAvailable_;
    // taking the labeled index
    firstAvailableBlock_ = *ret;
    return ret;
}

void FixedAllocator::Chunk::Deallocate(void* p, std::size_t blockSize) {
    assert(p >= pData_);

    unsigned char* toRelease = static_cast<unsigned char*>(p);
    // Alignment check
    assert((toRelease - pData_) % blockSize == 0);

    *toRelease = firstAvailableBlock_;
    firstAvailableBlock_ = static_cast<unsigned char>(
        (toRelease - pData_) / blockSize);
    // Truncation check
    assert(firstAvailableBlock_ == (toRelease - pData_) / blockSize);

    ++blocksAvailable_;
}

FixedAllocator::FixedAllocator(std::size_t blockSize)
    : blockSize_(blockSize)
    , allocChunk_(0)
    , deAllocChunk_(0)
{
    assert(blockSize_ > 0);
    
    std::size_t numBlocks = DEFAULT_CHUNK_SIZE / blockSize;
    if (numBlocks > UCHAR_MAX) numBlocks = UCHAR_MAX;
    else if (numBlocks == 0) numBlocks = 8 * blockSize;
    
    numBlocks_ = static_cast<unsigned char>(numBlocks);
    assert(numBlocks_ == numBlocks);
}

FixedAllocator::~FixedAllocator() {
    Chunks::iterator i = chunks_.begin();
    for (; i != chunks_.end(); ++i)
    {
       assert(i->blocksAvailable_ == numBlocks_);
       i->Release();
    }
}

void* FixedAllocator::Allocate() {
    if (allocChunk_ == nullptr || allocChunk_->blocksAvailable_ == 0) {
        // current Chunk is not useful
        for (auto iter = chunks_.begin(); ;iter++) {
            if (iter == chunks_.end()) {
                // no Chunk in vector is useful, need to allocate a new Chunk 
                chunks_.reserve(chunks_.size() + 1);
                Chunk newChunk;
                newChunk.Init(blockSize_, numBlocks_);
                chunks_.push_back(newChunk);
                allocChunk_ = &(chunks_.back());
                deAllocChunk_ = &(*chunks_.begin());
                break;
            }
            if (iter->blocksAvailable_ > 0) {
                // find one useful Chunk in vector
                allocChunk_ = &(*iter);
                break;
            }
        } 
    } 

    // use assert to check errors
    assert(allocChunk_ != nullptr && allocChunk_->blocksAvailable_ > 0);

    return allocChunk_->Allocate(blockSize_);
}

void FixedAllocator::Deallocate(void* p) {
    assert(!chunks_.empty() && 
        deAllocChunk_ >= &(*chunks_.begin()) &&
        deAllocChunk_ <= &(*chunks_.end()));

    deAllocChunk_ = LinearFind(p);
    assert(deAllocChunk_ != nullptr);

    DoDeAllocate(p);
}

FixedAllocator::Chunk* FixedAllocator::LinearFind(void* p) {
    const std::size_t ChunkLength = blockSize_ * numBlocks_; 
    assert(!chunks_.empty());
    for (auto iter = chunks_.begin(); ; iter++) {
        if (p >= iter->pData_ && p < iter->pData_ + ChunkLength) {
            return &(*iter);
        }
    } 
    assert(false); // never reaches this statement
    return nullptr;
}

void FixedAllocator::DoDeAllocate(void* p) {
    deAllocChunk_->Deallocate(p, blockSize_); 
    if (deAllocChunk_->blocksAvailable_ == numBlocks_) {
        // deAllocChunk_ is free and can be released
        Chunk& lastChunk = chunks_.back();

        if (&lastChunk == deAllocChunk_) {
            if (chunks_.size() > 1 && chunks_[chunks_.size()-2].blocksAvailable_ == numBlocks_) {
                lastChunk.Release();
                chunks_.pop_back();
                allocChunk_ = deAllocChunk_ = &(chunks_.front());
            }
            return;
        }

        if (lastChunk.blocksAvailable_ == numBlocks_) {
            // Two free blocks, discard one
            lastChunk.Release();
            chunks_.pop_back();
            allocChunk_ = deAllocChunk_;
        } else {
            // move the empty chunk to the end
            std::swap(*deAllocChunk_, lastChunk);
            allocChunk_ = &(chunks_.back());
        }
    }
}

SmallObjAllocator::SmallObjAllocator(
        std::size_t chunkSize, 
        std::size_t maxObjectSize)
    : pLastAlloc_(0), pLastDealloc_(0)
    , chunkSize_(chunkSize), maxObjectSize_(maxObjectSize) 
{   
}

namespace { // anoymous 

    struct CompareFixedAllocatorSize
        : std::binary_function<const FixedAllocator &, std::size_t, bool>
    {
        bool operator()(const FixedAllocator &x, std::size_t numBytes) const
        {
            return x.Blocksize() < numBytes;
        }
    };

} // anoymous namespace

void* SmallObjAllocator::Allocate(std::size_t numBytes)
{
    if (numBytes > maxObjectSize_) return operator new(numBytes);
    
    if (pLastAlloc_ && pLastAlloc_->Blocksize() == numBytes)
    {
        return pLastAlloc_->Allocate();
    }
    Pool::iterator i = std::lower_bound(pool_.begin(), pool_.end(), numBytes, 
                                        CompareFixedAllocatorSize());
    if (i == pool_.end() || i->Blocksize() != numBytes)
    {
        i = pool_.insert(i, FixedAllocator(numBytes));
        pLastDealloc_ = &*pool_.begin();
    }
    pLastAlloc_ = &*i;
    return pLastAlloc_->Allocate();
}

void SmallObjAllocator::Deallocate(void* p, std::size_t numBytes)
{
    if (numBytes > maxObjectSize_) return operator delete(p);

    if (pLastDealloc_ && pLastDealloc_->Blocksize() == numBytes)
    {
        pLastDealloc_->Deallocate(p);
        return;
    }
    Pool::iterator i = std::lower_bound(pool_.begin(), pool_.end(), numBytes, 
                                        CompareFixedAllocatorSize());
    assert(i != pool_.end());
    assert(i->Blocksize() == numBytes);
    pLastDealloc_ = &*i;
    pLastDealloc_->Deallocate(p);
}