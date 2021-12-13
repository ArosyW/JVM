//
// Created by Arosy on 2021/12/13.
//

#include "BytecodeInterpreter.h"
#include "BytecodeStream.h"
#include "CodeRunBase.h"

bool BytecodeInterpreter::run(JavaThread *javaThread, MethodInfo *methodInfo) {
    //取出字节码指令
    BytecodeStream *bytecodeStream = methodInfo->getAttributeInfo()->getCode();
    int index = 0;
    while (index < bytecodeStream->getLength()) {
        unsigned char c = bytecodeStream->readByOne(index);
        printf("指令字节：%X\n", c);
        CodeRunBase::run[c](javaThread, bytecodeStream, index); //解释指令
    }
    printf("===============执行方法结束 :%s =================\n", methodInfo->getMethodName().c_str());
    return true;
};