#ifndef _SMALL_OBJ_H
#define _SMALL_OBJ_H

namespace Mcd {
    class FixedAllocator {
    private:
        /*
            Chunk is continuous blocks*blockSize bytes, each blockSize is seen as a block
        */
        struct Chunk {
            /*
                The reason of using blocks as char type is very crucial,
                then it won't have any alignment problems.
            */
            void Init(std::size_t blockSize, unsigned char blocks);
            void* Allocate(std::size_t blockSize);
            void Deallocate(void* p, std::size_t blockSize);
            void Reset(std::size_t blockSize, unsigned char blocks);
            void Release();
            unsigned char* pData_;
            unsigned char
                // the address of first available block is pData_ + firstAvaileableBlocks_ * blockSize
                firstAvailableBlock_, 
                // number of available blocks
                blocksAvailable_;
        };

        /*
            member of FixedAllocator
            FixedAllocate owns a vector of Chunk, the size of vector can be increased and the limit is 
            only the system parameter.
        */
        std::size_t blockSize_; // passed to Chunk
        unsigned char numBlocks_; // passed to Chunk
        typedef vector<Chunk> Chunks;
        typedef vector<Chunk>::iterator IteratorChunks;
        Chunks chunks_; // FixedAllocator owns a vector of Chunk
        IteratorChunks allocChunk_; // available Chunk in vector
        IteratorChunks deAllocChunk_; // Chunk to release in vector

    public:
        // constructor
        explicit FixedAllocator(std::size_t blockSize = 0);
        FixedAllocator(const FixedAllocator&);
        FixedAllocator& operator=(const FixedAllocator&);

        // destructor
        ~FixedAllocator();

        // allocate a block of fixed size: blockSize_
        void* Allocate();
        void Deallocate(void* p);

        std::size_t Blocksize() const {
            return this->blockSize_; 
        }
    };
} // namespace Mcd

#endif