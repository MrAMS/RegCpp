# RegCpp

[中文](./README.cn.md)

tldr: A C++ 17 header-only library to write [Chisel](https://www.chisel-lang.org/)-style sequential logic `Reg`

## Overview

A `Reg` example:
```cpp
using WordT = uint32_t;
Reg<WordT> r1(0);
r1=r1+1;
for(int i=1;i<10;++i){
    SeqLogic::clock_step();
    assert(r1.out() == i);
}
```

Another `Reg` example:
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

A `RegEnable` example(a reg with enable signal):
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

A `ShiftReg` example:
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

## Purpose

A toy, try to automatically manages timing logic without having to manually manage.

## Usage

header-only library, just include `.hpp`

## Test

Install [XMake](https://github.com/xmake-io/xmake/), and run:
```shell
$ xmake build FrameworkTest
$ xmake run FrameworkTest
```