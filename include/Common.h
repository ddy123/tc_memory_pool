#pragma once
#include<cstddef>
#include<algorithm>

constexpr size_t ALIGNMENT =8;
constexpr size_t MAX_BYTES=256*1024;
constexpr size_t FREE_LIST_SIZE=MAX_BYTES/ALIGNMENT;

class SizeClass{
public:
    //向上往8字节对齐
    static size_t roundUp(size_t bytes){
        return (bytes+ALIGNMENT-1)&~(ALIGNMENT-1);
    }

    static size_t getIndex(size_t bytes){
        //分配最小内存为8字节，0-8字节都要分配8字节
        bytes=std::max(bytes,ALIGNMENT);
        //向上取整并转换为数组下标
        return (bytes+ALIGNMENT-1)/ALIGNMENT-1;
    }

};