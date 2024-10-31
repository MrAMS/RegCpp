#pragma once

#include <cstdint>

class Utils{
    static constexpr uint32_t log2(uint64_t num){
        for(int i=0;num!=0;++i){
            if(num&1) return i;
            num=num>>1;
        }
        return 0;
    }
};