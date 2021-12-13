//
// Created by Arosy on 2021/12/13.
//

#include "CodeRunBase.h"
typedef void (*CODERUN)(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
CODERUN CodeRunBase::run[256];
void CodeRunBase::initCodeRun(){
    run[NOP] = funcNOP; // NOP枚举值为0
}
void CodeRunBase::funcNOP(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
//nothing to do
}