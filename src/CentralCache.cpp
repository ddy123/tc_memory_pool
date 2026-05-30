#include<cassert>
#include<thread>
#include "../include/CentralCache.h"
#include "../include/PageCache.h"

namespace ddy_memoryPool{
//每次从PageCache获取span大小(以页为单位)
static const size_t SPAN_PAGES=8;

void CentralCache::returnRange(void* start,size_t size,size_t index){
    //当索引大于等于FREE_LIST_SIZE时，说明内存过大应直接向系统归还
    if(!start||indxe>=FREE_LIST_SIZE){
        return;
    }
    while(locks_[index].test_and_set(std::memory_order_acquire)){
        std::this_thread::yield();
    }
    try{
        //找到要归还的链表最后一个节点
        void* end=start;
        size_t count=1;
        while(*reinterpret_cast<void**>(end)!=nullptr&&count<size){
            end=*reinterpret_cast<void**>(end);
            count++;
        }
        //将归还的链表连接到中心缓存的链表头部
        void* current=centralFreeList_[index].load(std::memory_order_relaxed);
        *reinterpret_cast<void**>(end)=current;// 将原链表头接到归还链表的尾部
        centralFreeList_[index].store(start,std::memory_order_release);// 将归还的链表头设为新的链表头
    }ctach(...){

    }
    locks_[index].clear(std::memory_order_release);
}

}