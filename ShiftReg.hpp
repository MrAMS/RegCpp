#pragma once

#include "Logic.hpp"
#include "Reg.hpp"
#include "Wire.hpp"
#include <cstdint>

template<typename T, uint32_t n>
class ShiftReg: ComLogic{
public:
    ShiftReg(){
        static_assert(n>0);
        for(uint32_t i=1;i<n;++i)
            regs[i] = regs[i-1];
    }
    ShiftReg(const Reg<T>& in){
        static_assert(n>0);
        regs[0] = in;
        for(uint32_t i=1;i<n;++i)
            regs[i] = regs[i-1];
    }
    
    template<typename T2>
    ShiftReg& in(const T2& next){
        regs[0] = next;
        return *this;
    }

    const Reg<T>& out() const{
        return regs[n-1];
    }
private:
    Reg<T> regs[n];
};
