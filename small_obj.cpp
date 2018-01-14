#include "small_obj.h"
#include <cassert>

using namespace Mcd;

void FixedAllocator::Chunk::Init(std::size_t blockSize, unsigned char blocks) {
    assert(blocks > 0 && blockSize > 0); 
    // check overflow
    assert((blocks * blockSize) / blockSize == blocks);
    pData_ = new char[blocks * blockSize];
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

void* FixedAllocator::Allocate() {
    
}