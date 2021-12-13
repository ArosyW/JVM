//
// Created by Arosy on 2021/12/13.
//

#ifndef JVM_BYTECODEINTERPRETER_H
#define JVM_BYTECODEINTERPRETER_H

#include "../runtime/JavaThread.h"
#include "../oop/MethodInfo.h"

/**
 * 字节码解释器
 */
class BytecodeInterpreter {
public:
    static bool run(JavaThread* javaThread,MethodInfo* methodInfo);
};


#endif //JVM_BYTECODEINTERPRETER_H
