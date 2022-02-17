//
// Created by Arosy on 2022/2/16.
//

#include "StubRoutines.h"
unsigned long StubRoutines::callStubEntry = 0;

::CallStub StubRoutines::CallStub() {
    return (::CallStub)callStubEntry;
}
void StubRoutines::GenerateInitial() {
    callStubEntry = Asm::pc();
    char* temp = (char *)callStubEntry;
    Asm::saveCaller(); //保存调用者栈基地址
    Asm::movRcxByRsp(16);
    Asm::shlRcx(3);
    Asm::subRspByRcx();
//    __asm__ volatile (
//    "pushq  %rbp;"
//    "movq   %rsp, %rbp;"
//    "pushq  %rcx;"
//    //分配栈空间
//    " movq   0x10(%rsp), %rcx;"
//    "shll   $0x3, %rcx;"
//    "subq   %rcx, %rsp;"
//
//
//    "pushq  %rcx;"
//    );
}