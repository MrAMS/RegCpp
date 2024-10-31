#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include <cstdint>

#include "Logic.hpp"
#include "Reg.hpp"
#include "ShiftReg.hpp"
#include "Wire.hpp"
#include "ALU.hpp"

#define OUT_REQUIRE(F) REQUIRE((F)())

TEST_CASE( "Reg work good", "[Reg]" ) {
    Reg<uint32_t> r1(0);
    r1=r1+1;
    OUT_REQUIRE(r1==0);
    OUT_REQUIRE(r1==r1.out());
    OUT_REQUIRE(r1==(*r1));

    SeqLogic::clock_step();
    OUT_REQUIRE(r1==1);
    REQUIRE(SeqLogic::cycs==1);

    SeqLogic::clock_step();
    OUT_REQUIRE(r1==2);
    REQUIRE(SeqLogic::cycs==2);

    r1 = 0;
    SeqLogic::clock_step();
    OUT_REQUIRE(r1==0);

    SeqLogic::clock_step();
    OUT_REQUIRE(r1==0);

    Reg<uint32_t> r2(0, WireFunc{return (*r1) == 1;});
    // Reg<uint32_t> r2(0, (r1==1));
    r1 = r1+1;
    SeqLogic::clock_step();
    OUT_REQUIRE(r1==1);
    OUT_REQUIRE(r2==0);

    SeqLogic::clock_step();
    OUT_REQUIRE(r1==2);
    OUT_REQUIRE(r2==1);
}

TEST_CASE( "RegEnable work good", "[RegEnable]" ) {
    Reg<uint32_t> r1(0);
    r1=r1+1;
    Reg<uint32_t> r2(0);
    Reg<uint32_t> r3(1);

    r2.en((r1==1) && (r3==1)).in(r2+1); // must en before in
    OUT_REQUIRE(r1==0);
    OUT_REQUIRE(r2==0);

    SeqLogic::clock_step();
    OUT_REQUIRE(r1==1);
    OUT_REQUIRE(r2==0);

    SeqLogic::clock_step();
    OUT_REQUIRE(r1==2);
    OUT_REQUIRE(r2==1);

    SeqLogic::clock_step();
    OUT_REQUIRE(r1==3);
    OUT_REQUIRE(r2==1);

    SeqLogic::clock_step();
    OUT_REQUIRE(r1==4);
    OUT_REQUIRE(r2==1);

    r2.in(4);
    SeqLogic::clock_step();
    OUT_REQUIRE(r1==5);
    OUT_REQUIRE(r2==1); // set can not overwrite enable

    r2.en(r1==5);
    SeqLogic::clock_step();
    OUT_REQUIRE(r1==6);
    OUT_REQUIRE(r2==2);

    r2.en(r1==6).in(0);
    SeqLogic::clock_step();
    OUT_REQUIRE(r1==7);
    OUT_REQUIRE(r2==0);
}

TEST_CASE( "ALU work good", "[ALU]" ) {
    using WordT = uint32_t;
    Reg<WordT> a(0);
    a = a+1;
    Reg<WordT> b(1);
    b = b*2;
    Reg<ALU<WordT>::OP> op(ALU<WordT>::ADD);
    uint32_t* alu_res1 = nullptr;
    ALU<WordT> alu(&a, &b, &op, &alu_res1);
    Reg<WordT> alu_res2(1, a+b);
    Reg<WordT> alu_res3(0);
    ALU<WordT> alu2(a, b, op, alu_res3);

    for(int i=0;i<5;++i){
        const auto val_a=(*a), val_b=(*b);

        SeqLogic::clock_step();
        
        REQUIRE((*alu_res1) == val_a+val_b);
        OUT_REQUIRE((*alu_res1) == alu_res2);
        OUT_REQUIRE((*alu_res1) == alu_res3);
    }
}

TEST_CASE( "ShiftReg work good", "[ShiftReg]" ) {
    using WordT = uint32_t;
    const auto ShiftN = 3;
    SECTION( "use ref" ){
        Reg<WordT> in(0);
        in = in+1;
        ShiftReg<WordT, ShiftN> shift(in);
        for(int i=0;i<ShiftN;++i){
            SeqLogic::clock_step();
        }
        for(int i=0;i<ShiftN;++i){
            OUT_REQUIRE(shift.out()==i);
            SeqLogic::clock_step();
        }
    }
    SECTION( "use in/out" ){
        ShiftReg<WordT, ShiftN> shift;
        for(int i=0;i<ShiftN;++i){
            shift.in(i);
            SeqLogic::clock_step();
        }
        for(int i=0;i<ShiftN;++i){
            OUT_REQUIRE(shift.out()==i);
            SeqLogic::clock_step();
        }
    }
}