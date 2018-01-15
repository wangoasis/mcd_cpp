#ifndef _SMALL_OBJ_H
#define _SMALL_OBJ_H

#include <cstddef>
#include <vector>
using std::vector;

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

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
        Chunks chunks_; // FixedAllocator owns a vector of Chunk
        Chunk* allocChunk_; // available Chunk in vector
        Chunk* deAllocChunk_; // Chunk to release in vector

        /*
            private helper function
        */
        Chunk* LinearFind(void* p);
        void DoDeAllocate(void* p);

    public:
        // constructor
        explicit FixedAllocator(std::size_t blockSize = 0);

        // destructor
        ~FixedAllocator();

        // allocate a block of fixed size: blockSize_
        void* Allocate();
        void Deallocate(void* p);

        std::size_t Blocksize() const {
            return this->blockSize_; 
        }
    };


    class SmallObjAllocator {
    public:
        SmallObjAllocator(
            std::size_t chunkSize, 
            std::size_t maxObjectSize);
    
        void* Allocate(std::size_t numBytes);
        void Deallocate(void* p, std::size_t size);
    
    private:
        SmallObjAllocator(const SmallObjAllocator&);
        
        typedef std::vector<FixedAllocator> Pool;
        Pool pool_;
        FixedAllocator* pLastAlloc_;
        FixedAllocator* pLastDealloc_;
        std::size_t chunkSize_;
        std::size_t maxObjectSize_;
    };

    template <
        std::size_t chunkSize = DEFAULT_CHUNK_SIZE,
        std::size_t maxSmallObjectSize = MAX_SMALL_OBJECT_SIZE
    >
    class SmallObject {
                
        struct MySmallObjAllocator : public SmallObjAllocator
        {
            MySmallObjAllocator() 
            : SmallObjAllocator(chunkSize, maxSmallObjectSize)
            {}
        };

    private:
        MySmallObjAllocator* obj;
        
    public:
        SmallObject() {
            obj = new MySmallObjAllocator();
        }

        void* newMemory(std::size_t size)
        {
            return obj->Allocate(size);
        }

        void deleteMemory(void* p, std::size_t size)
        {
            
            obj->Deallocate(p, size);
        }

        ~SmallObject() {}
    };
} // namespace Mcd

#endif