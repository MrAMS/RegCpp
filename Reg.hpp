#pragma once

#include "Logic.hpp"
#include <functional>
#include <cassert>

template<typename T>
class Reg: public SeqLogic{
public:
    Reg()=default;
    Reg(std::function<T()> next){
        in(next);
    }
    Reg(T init){
        val_out = val_eval = init;
    }
    Reg(T init, const std::function<T()>& next){
        val_out = val_eval = init;
        in(next);
    }
    Reg(const std::function<T()>& next, const std::function<bool()>& enable){
        in(next);
        en(enable);
    }
    Reg(T init, const std::function<T()>& next, const std::function<bool()>& enable){
        val_out = val_eval = init;
        in(next);
        en(enable);
    }

    Reg& in(const std::function<T()>& next){
        infunc = next;
        if(enfunc()){
            val_eval = infunc();
        }
        return *this;
    }
    const std::function<T()>& operator=(const std::function<T()>& next){
        in(next);
        return next;
    }

    Reg& in(const T& next){
        if(enfunc()){
            infunc = [next](){return next;};
            val_eval = infunc();
        }
        return *this;
    }
    const T& operator=(const T& next){
        in(next);
        return next;
    }
    
    Reg& in(const Reg& next){
        assert((&next)!=this);
        infunc = [&next](){return next.out();};
        if(enfunc()){
            val_eval = infunc();
        }
        return *this;
    }
    const Reg& operator=(const Reg& next){
        in(next);
        return next;
    }

    Reg& en(const std::function<bool()>& enable){
        enfunc = enable;
        return *this;
    }

    T out() const{
        return val_out;
    }
    T operator*() const{
        return val_out;
    }

    T* outp(){
        return &val_out;
    }
    T* operator&(){
        return &val_out;
    }

    // operator T() const{
    //     return out();
    // }
    
protected:
    std::function<T()> infunc=[&](){return val_eval;};
    std::function<bool()> enfunc=[](){return true;};

    T val_eval, val_out;
    void eval() override{
        if(enfunc()){
            val_eval = infunc();
        }
    }
    void update() override{
        val_out = val_eval;
    }
private:
    // no move assignment
    Reg<T>& operator=(Reg<T>&& other) noexcept{

    }
};

#define reg_comparison_operator_overloading(SYMBOL) \
    template<typename T, typename T2> \
    inline const std::function<bool()> operator SYMBOL(const Reg<T>& reg, const T2& val){ \
        return [&reg, val](){return reg.out() SYMBOL val;}; \
    } \
    template<typename T, typename T2> \
    inline const std::function<bool()> operator SYMBOL(const T2& val, const Reg<T>& reg){ \
        return reg SYMBOL val; \
    } \
    template<typename T, typename T2> \
    inline const std::function<bool()> operator SYMBOL(const Reg<T>& regl, const Reg<T2>& regr){ \
        return [&regl, &regr](){return regl.out() SYMBOL regr.out();}; \
    }

reg_comparison_operator_overloading(==)
reg_comparison_operator_overloading(!=)
reg_comparison_operator_overloading(<)
reg_comparison_operator_overloading(<=)
reg_comparison_operator_overloading(>)
reg_comparison_operator_overloading(>=)

#define reg_binary_arithmetic_operator_overloading(SYMBOL) \
    template<typename T, typename T2> \
    inline const std::function<T()> operator SYMBOL(const Reg<T>& reg, const T2& val){ \
        return [&reg, val](){return reg.out() SYMBOL val;}; \
    } \
    template<typename T, typename T2> \
    inline const std::function<T()> operator SYMBOL(const T2& val, const Reg<T>& reg){ \
        return reg SYMBOL val; \
    } \
    template<typename T> \
    inline const std::function<T()> operator SYMBOL(const Reg<T>& regl, const Reg<T>& regr){ \
        return [&regl, &regr](){return regl.out() SYMBOL regr.out();}; \
    }

reg_binary_arithmetic_operator_overloading(+)
reg_binary_arithmetic_operator_overloading(-)
reg_binary_arithmetic_operator_overloading(*)
reg_binary_arithmetic_operator_overloading(/)
reg_binary_arithmetic_operator_overloading(%)

reg_binary_arithmetic_operator_overloading(^)
reg_binary_arithmetic_operator_overloading(&)
reg_binary_arithmetic_operator_overloading(|)
