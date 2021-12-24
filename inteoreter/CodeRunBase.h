//
// Created by Arosy on 2021/12/13.
//

#ifndef JVM_CODERUNBASE_H
#define JVM_CODERUNBASE_H
#include "ByteCode.h"
#include "BytecodeStream.h"
#include "../runtime/JavaThread.h"

typedef void (*CODERUN)(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);

class CodeRunBase {
public:
    static CODERUN run[256];
    static void initCodeRun();
    static void funcNOP(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
    static void funcGETSTATIC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
    static void funcPUTSTATIC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
    static void funcLDC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
    static void funcICONST0(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
    static void funcDUP(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
};


#endif //JVM_CODERUNBASE_H
