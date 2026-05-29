#incldue "../include/ThreadCache.h"
#incldue "../include/MemoryCache.h"

namespace ddy_memoryPool{
    void* ThreadCache::allocate(size_t size){
        //处理0大小的分配请求
        if(size==0){
            //至少分配一个对齐大小
            size=ALIGNMENT;
        }
        if(size>MAX_BYTES){
            //大对象直接从系统分配
            return malloc(size);
        }
        size_t index=SizeClass::getIndex(size);
        if(void* ptr=freeList_[index]){
            //将freeList_[index]指向下一个内存块
            freeList_[index]=*reinterpret_cast<void**>(ptr);
            freeListSize_[index]--;
            return ptr;
        }
        //如果线程本地自由链表为空，则从中心缓存获取一批内存
        return fetchFromCentralCache(index);
    }


}