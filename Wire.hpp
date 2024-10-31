#pragma once

#include "Logic.hpp"

#include <functional>
#include <vector>

// C++ lambda capture by reference
#define WireFunc [&]()

#define wire_comparison_operator_overloading(SYMBOL) \
    template<typename T, typename T2> \
    inline const std::function<bool()> operator SYMBOL(const std::function<T()>& wire, const T2& val){ \
        return [&](){return wire() SYMBOL val;}; \
    } \
    template<typename T, typename T2> \
    inline const std::function<bool()> operator SYMBOL(const T2& val, const std::function<T()>& wire){ \
        return wire SYMBOL val; \
    } \
    template<typename T, typename T2> \
    inline const std::function<bool()> operator SYMBOL(const std::function<T()>& wirel, const std::function<T2()>& wirer){ \
        return [&](){return wirel() SYMBOL wirer();}; \
    }

wire_comparison_operator_overloading(==)
wire_comparison_operator_overloading(!=)
wire_comparison_operator_overloading(&&)
wire_comparison_operator_overloading(||)

#define wire_binary_arithmetic_operator_overloading(SYMBOL) \
    template<typename T, typename T2> \
    inline const std::function<T()> operator SYMBOL(const std::function<T()>& wire, const T2& val){ \
        return [&](){return wire() SYMBOL val;}; \
    } \
    template<typename T, typename T2> \
    inline const std::function<T()> operator SYMBOL(const T2& val, const std::function<T()>& wire){ \
        return wire SYMBOL val; \
    } \
    template<typename T> \
    inline const std::function<T()> operator SYMBOL(const std::function<T()>& wirel, const std::function<T()>& wirer){ \
        return [&](){return wirel() SYMBOL wirer();}; \
    }

wire_binary_arithmetic_operator_overloading(+)
wire_binary_arithmetic_operator_overloading(-)
wire_binary_arithmetic_operator_overloading(*)
wire_binary_arithmetic_operator_overloading(/)
wire_binary_arithmetic_operator_overloading(%)

wire_binary_arithmetic_operator_overloading(^)
wire_binary_arithmetic_operator_overloading(&)
wire_binary_arithmetic_operator_overloading(|)

/**
 * 目前的Wire是依赖C++现有的lambda函数，想做单独的一个Wire类，
 * 可以动态改变Wire的值，并根据Wire的改变更新所依赖的所有Wire，
 * 但是我感觉其实好像没必要，这个库的目的是为了自动管理时序赋值、
 * 方便用cpp写模拟器，而不是重新发明和实现SystemC，我发现其实
 * 可以将各个模块封装成一个类，重载eval使用cpp编写行为建模代码，
 * 然后各个模块只暴露Reg给外部，通过Reg互相进行通信和同步，这样
 * 既可以获得cpp行为建模的便利性，又可以利用Reg自动管理时序赋值。
 * 
 */


// template<typename T>
// class Wire: public ComLogic{
// public:
//     Wire()=default;
//     Wire(const Wire& next){
//         in(next);
//     }
//     Wire(const T& next){
//         in(next);
//     }

//     T out() const{
//         return eval_wire();
//     }
//     T operator *() const{
//         return out();
//     }
// protected:
//     T val_eval;
//     bool evaled;
//     std::function<T()> evalfunc=[&](){return val_eval;};

//     void update() override{
//         evaled = false;
//     }
// private:
//     T eval_wire(){
//         if(evaled) return val_eval;
//         val_eval = evalfunc();
//         evaled = true;
//         return val_eval;
//     }

//     // 暂不支持Wire创建后修改，因为需要做根据依赖更新其他Wire
//     Wire& in(const Wire& next){
//         evalfunc = next.evalfunc;
//         evaled = false;
//         return *this;
//     }
//     const Wire& operator =(const Wire& next){
//         in(next);
//         return next;
//     }

//     Wire& in(const T& next){
//         evalfunc = [=](){return next;};
//         evaled = false;
//         return *this;
//     }
//     const T& operator =(const T& next){
//         in(next);
//         return next;
//     }
// };