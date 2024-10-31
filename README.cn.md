# RegCpp

太长不读：一个C++17头文件类库，写出类似Chisel风格的时序逻辑`Reg`

## 一览

一个使用`Reg`的例子:
```cpp
using WordT = uint32_t;
Reg<WordT> r1(0);
r1=r1+1;
for(int i=1;i<10;++i){
    SeqLogic::clock_step();
    assert(r1.out() == i);
}
```

另外一个`Reg`的例子:
```cpp
using WordT = uint32_t;
Reg<WordT> r1(0);
r1=r1+1;
Reg<WordT> r2(r1*2);
assert(r1.out()==0);
SeqLogic::clock_step();
for(int i=1;i<10;++i){
    assert(r1.out() == i);
    assert(r2.out() == (i-1)*2);
    SeqLogic::clock_step();
}
```

一个`RegEnable`的例子（带使能信号的寄存器）:
```cpp
using WordT = uint32_t;
Reg<WordT> r1(0);
r1=r1+1;
Reg<WordT> r2(r1*2, r1<=5);
assert(r1.out()==0);
SeqLogic::clock_step();
for(int i=1;i<10;++i){
    assert(r1.out() == i);
    assert(r2.out() == i>5?10:((i-1)*2));
    SeqLogic::clock_step();
}
```

一个`ShiftReg`的例子（移位寄存器）:
```cpp
using WordT = uint32_t;
const auto ShiftN = 3;
Reg<WordT> r1(0);
r1=r1+1;
ShiftReg<WordT, ShiftN> shift;
shift.in(r1);
for(int i=0;i<ShiftN;++i){
    SeqLogic::clock_step();
}
for(int i=0;i<ShiftN;++i){
    assert(shift.out().out()==i);
    SeqLogic::clock_step();
}
```

## 目的

C++玩具，使用框架自动管理时序逻辑，而无需手动维护时钟和并行赋值。

## Usage

header-only library, just include `.hpp`

## Test

Install [XMake](https://github.com/xmake-io/xmake/), and run:
```shell
$ xmake build FrameworkTest
$ xmake run FrameworkTest
```