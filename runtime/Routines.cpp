//
// Created by Arosy on 2022/2/16.
//

#include "Routines.h"
unsigned long Routines::callStubEntry = 0;

::CallStub Routines::CallStub() {
    return (::CallStub)callStubEntry;
}
void Routines::GenerateInitial() {
    InitCallStub();
    InitEntryPoint();
}

void Routines::InitCallStub() {
    callStubEntry = Asm::pc();
    Asm::saveCaller(); //保存调用者栈基地址
    Asm::pushOrPop(0x51);//rcx
    Asm::pushOrPop(0x57);//保存rdi、rsi、rbx寄存器
    Asm::pushOrPop(0x56);
    Asm::pushOrPop(0x53);
    //分配栈空间
    Asm::movRcxByRsp(0x10);
    Asm::shlRcx(3);
    Asm::subRspByRcx();
    Asm::movRcxByRsp(0x10);//保存入参数量
    Asm::dec(0xc9);
    Asm::CallStub();
    /**
     * 所生成的汇编代码如下
     */
//    __asm__ volatile (
//    "push  rbp\n"
//    "mov  rbp,rsp\n"
//    "push rcx\n"
//    "push rdi\n"
//    "push rsi\n"
//    "push rbx\n"
//    //分配栈空间start
//    "mov rcx ,[rsp+0x10]\n"
//    "shl rcx,3\n"
//    "sub rsp,rcx"
//    //分配栈空间end
//    "mov rcx ,[rsp+0x10]\n"
//    "dec rcx"
//    "i:test rcx rcx\n"
//    "je o\n"
//    "xor rbx,rbx"
//    //循环入参start
//    "shl rcx,3"
//    "mov [rsp+rcx],r9"
//    "shr rcx,3"
//    "dec rcx"
//    "inc r9"
//    "jne i"
//    "0:mov rsp,r9"
//    //循环入数end
//    "mov rcx ,[rsp+0x10]"
//    "mov rbx,rbp"
//    "call [r8d]"
//    //todo return
//    );
}

void Routines::InitEntryPoint() {
    __asm__ volatile (
    "pop rax" //保存返回指令地址
    "lea rdi,[rsp+8+rcx*8]" //保存第一个参数的地址
    //todo
    );
}





