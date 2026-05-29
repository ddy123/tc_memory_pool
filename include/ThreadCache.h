#pragma once
#include<array>
#include<cstddef>
#include<Common.h>

class ThreadCache{
    public:
        ThreadCache* getInstance();
        void* allocate(size_t size);
        void deallocate(void* ptr,size_t size);
    private:
        ThreadCache();
    
    private:
        void* fetchFromCentralCache(size_t index);
        void returnToCentralCache(void* start,size_t size);

        size_t getBatchNum(size_t size);
        bool shouldReturnToCentralCache(size_t index);
    
    private:
        std::array<void*,FREE_LIST_SIZE> freeList_;
        std::array<size_t,FREE_LIST_SIZE> freeListSize_;
}