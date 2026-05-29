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
            //将freeList_[index]指向下一个内存块,把ptr转为二级指针，这样的话ptr指向的内存块的前八字节便是一级指针，对ptr解引用则得到一级指针值
            freeList_[index]=*reinterpret_cast<void**>(ptr);
            freeListSize_[index]--;
            return ptr;
        }
        //如果线程本地自由链表为空，则从中心缓存获取一批内存
        return fetchFromCentralCache(index);
    }
    void ThreadCache::deallocate(void* ptr,size_t size){
        if(ptr==nullptr){
            return;
        }
        if(size>MAX_BYTES){
            free(ptr);
            return;
        }
        size_t index=SizeClass::getIndex(size);

        //插入到线程本地自由链表
        *reinterpret_cast<void**>(ptr)=freeList_[index];
        freeList_[index]=ptr;

        //更新自由链表大小,增加对应大小类的自由链表大小
        freeListSize_[index]++

        //判断是否需要将部分内存回收给中心缓存
        if (shouldReturnToCentralCache(index))
    {
        returnToCentralCache(freeList_[index], size);
    }

    }


}