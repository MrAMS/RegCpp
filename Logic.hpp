#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

#include <cstdio>

class SeqLogic{
public:
    SeqLogic(){
        all.push_back(this);
    }
    ~SeqLogic(){
        auto it = std::find(all.begin(), all.end(), this);
        if(it != all.end()) all.erase(it);
        else assert(0);
    }
    static std::vector<SeqLogic*>& get_all_Logics(){
        return all;
    }
    static void clock_step(){
        for(auto& m : all){
            m->eval();
        }
        for(auto& m : all){
            m->update();
        }
        cycs += 1;
    }
    static inline uint64_t cycs=0;
protected:
    virtual void eval()=0;
    virtual void update()=0;
private:
    static inline std::vector<SeqLogic*> all;
};

class ComLogic: public SeqLogic{
protected:
    void eval() override{
        return;
    }
    void update() override{
        return;
    }
};