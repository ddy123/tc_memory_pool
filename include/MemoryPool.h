#pragma once
#include<cstddef>
#include "ThreadChche.h"
namespace ddy_memoryPool{
class MemoryPool{
public:
    static void* allocate(size_t size){
        return ThreadCache::getInstance()->allocate(size);
    }
    static void deallocate(void* ptr,size_t size){
        return ThreadCache::getInstance()->deallocate(ptr,size);
    }
};

}
