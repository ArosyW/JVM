//
// Created by Arosy on 2022/2/16.
//

#ifndef JVM_ASM_H
#define JVM_ASM_H


class Asm {
public:
    static unsigned long address;
    static void init();
    static unsigned long pc();
    static void saveCaller();
    static void movRcxByRsp(int offset);
    static void shlRcx(int bit);
    static void subRspByRcx();
    static void pushOrPop(int registers);
    static void dec(int registers);
    static void CallStub();





};


#endif //JVM_ASM_H
