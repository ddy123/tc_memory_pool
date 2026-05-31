#pragma once
#include "Common.h"
#include<map>
#include<mutex>
namespace ddy_memoryPool{
    class PageCache{
    public:
        static const size_t PAGE_SIZE=4096;//4k页大小
        static PageCache& getInstance(){
            static PageCache instance;
            return instance;
        }
        //从系统获取内存页
        void* allocateSpan(size_t numPages);
        //将内存页返回系统
        void deallocateSpan(void* ptr,size_t numPages);
    private:
        PageCache()=default;
        //向系统申请内存
        void* systemAlloc(size_t numPages);
    private:
        Struct Span{
            void* pageAddr;//页起始地址
            size_t numPages;//页数
            Span* next;//链表指针
        };
        std::mutex mutex_;//保护页缓存的互斥锁
        std::map<size_t,Span*> freeSpans_;//空闲内存页的映射，key为页数，value为对应页数的空闲内存块列表
        std::map<void*,Span*> spanMap_;//页号到Span的映射，用于回收
    };
}