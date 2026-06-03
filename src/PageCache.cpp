#include<PageCache.h>
#include<sys/mman.h>
#include<cstring>
namespace ddy_memoryPool{
    void* PageCache::allocateSpan(size_t numPages){
        std::lock_guard<std::mutex> lock(mutex_);
        //查找合适的空闲span
        //lower_bound返回第一个大于等于numPages的key的迭代器
        auto it=freeSpans_.lower_bound(numPages);
        if(it!=freeSpans_.end()){
            Span* span=it->second;
            //将取出的span从原有的空闲链表freeSpans_中移除
            if(span->next){
                freeSpans_[it->first]=span->next;
            }else{
                freeSpans_.erase(it);
            }
            //如果span大于需要的numPages，则将剩余部分重新加入freeSpans_
            if(span->numPages>numPages){
                Span* remainSpan=new Span;
                remainSpan->pageAddr=static_cast<char*>(span->pageAddr)+numPages*PAGE_SIZE;
                remainSpan->numPages=span->numPages-numPages;
                remainSpan->next=nullptr;
                //将超出部分放回空闲span*列表头部
                auto& list=freeSpans_[remainSpan->numPages];
                remainSpan->next=list;
                list=remainSpan;
                span->numPages=numPages;
            }
            //记录span信息用于回收
            spanMap_[span->pageAddr]=span;
            return span->pageAddr;
            
        }
        //没有合适的span向系统申请
        void* memory=systemAlloc(numPages);
        if(!memory) return nullptr;
        
        //创新新的span，向系统申请
        Span* span=new Span;
        span->pageAddr=memory;
        span->numPages=numPages;
        span->next=nullptr;
        //记录span信息用于回收
        spanMap_[memory]=span;
        return memory;
    }
    void* PageCache::systemAlloc(size_t numPages){
        size_t size=numPages*PAGE_SIZE;
        //使用mmap分配内存
        void* ptr=mmap(nullptr,size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(ptr==MAP_FAILED){
            return nullptr;
        }
        //清零内存
        memset(ptr,0,size);
        return ptr;
    }
}