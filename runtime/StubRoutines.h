//
// Created by Arosy on 2022/2/16.
//
#include "../asm/Asm.h"
#include "../oop/MethodInfo.h"
#include "../runtime/JavaThread.h"

#ifndef JVM_STUBROUTINES_H
#define JVM_STUBROUTINES_H

typedef void (*CallStub)(unsigned long link,
                         int* result,
                         int resultType,
                         MethodInfo* method,
        unsigned long entryPoint,
        int* parameters,
        int parametersCount,
        JavaThread* javaThread
        );
class StubRoutines {
public:
    static unsigned long callStubEntry;
    static ::CallStub CallStub();
    static void GenerateInitial();

};


#endif //JVM_STUBROUTINES_H
