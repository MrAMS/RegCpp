#pragma once

#include "Logic.hpp"
#include "Reg.hpp"
#include "Wire.hpp"

#include <type_traits>
#include <cassert>

template<typename T>
class ALU{
public:
    enum OP{
        ADD, SUB, SLL, SRL, SRA,
        AND, OR, XOR,
        LT, LTU
    };
private:
    const T* a;
    const T* b;
    const OP* op;
    Reg<T> rres;
public:
    ALU()=delete;
    ALU(const T* ia, const T* ib, const OP* iop, T** ores):a(ia), b(ib), op(iop){
        rres.in(WireFunc{return ALU::calc(*a, *b, *op);});
        (*ores) = &rres;
    }
    ALU(const Reg<T>& ira, const Reg<T>& irb, const Reg<OP>& irop, Reg<T>& orc){
        orc = WireFunc{return ALU::calc(*ira, *irb, *irop);};
    }
    static T calc(const T& A, const T& B, const OP& op){
        switch (op) {
            case ADD:
                return static_cast<std::make_signed_t<T>>(A)+static_cast<std::make_signed_t<T>>(B);
                break;
            case SUB:
                return static_cast<std::make_signed_t<T>>(A)-static_cast<std::make_signed_t<T>>(B);
                break;
            case SLL:
                return static_cast<std::make_unsigned_t<T>>(A)<<B;
                break;
            case SRL:
                return static_cast<std::make_unsigned_t<T>>(A)>>B;
                break;
            case SRA:
                return static_cast<std::make_signed_t<T>>(A)>>B;
                break;
            case AND:
                return A&B;
                break;
            case OR:
                return A|B;
                break;
            case XOR:
                return A^B;
                break;
            case LT:
                return static_cast<std::make_signed_t<T>>(A) < static_cast<std::make_signed_t<T>>(B);
                break;
            case LTU:
                return static_cast<std::make_unsigned_t<T>>(A) < static_cast<std::make_unsigned_t<T>>(B);
                break;
            default:
                assert(0);
        }
    }
};
