//
// Created by Arosy on 2021/12/11.
//

#include "JavaNativeInterface.h"
#include "../oop/InstanceKlass.h"
#include "../inteoreter/BytecodeInterpreter.h"

MethodInfo *JavaNativeInterface::getMethod(InstanceKlass *klass, string name, string descriptor) {
    for (int i = 0; i < klass->getMethodCount(); i++) { //遍历这个klass的所有方法
        MethodInfo *m = (klass->getMethodInfo() + i);
        string nameMethod = klass->getConstantPool()->data[m->getNameIndex()];
        string descMethod = klass->getConstantPool()->data[m->getDescriptorIndex()];
        if (name == nameMethod && descriptor == descriptor) { // 如果这个方法是要找的就return
            return m;
        }
    }
    return NULL;
};
void JavaNativeInterface::callStaticMethod(JavaThread* javaThread, MethodInfo *method) {
    printf("===============执行方法开始 :%s =================\n", method->getMethodName().c_str());
    JavaVFrame *javaVFrame = new JavaVFrame;//马上要执行方法了，先创建栈帧
    javaThread->stack.push(javaVFrame);//栈帧push进线程的栈空间
    BytecodeInterpreter::run(javaThread, method); // 执行方法
    javaThread->stack.pop();//将执行完成的栈桢弹出栈空间
    delete javaVFrame; //释放栈桢内存空间
}

void JavaNativeInterface::callVirtual(JavaThread* javaThread, MethodInfo *method,int paramCount ,char** params) {
    printf("===============执行方法开始 :%s =================\n", method->getMethodName().c_str());
    JavaVFrame *javaVFrame = new JavaVFrame;//马上要执行方法了，先创建栈帧
    for (int i = 1; i <= paramCount; ++i) { //将参数写入局部变量表
        javaVFrame->locals[i] = (CommonValue *) (*(params + i - 1));
    }
    javaVFrame->locals[0] = (CommonValue *)*(params + paramCount);//非静态方法第一个参数为this指针
    javaThread->stack.push(javaVFrame);//栈帧push进线程的栈空间
    BytecodeInterpreter::run(javaThread, method); // 执行方法
    javaThread->stack.pop();//将执行完成的栈桢弹出栈空间
    delete javaVFrame; //释放栈桢内存空间
}

void JavaNativeInterface::callSpecial(JavaThread* javaThread, MethodInfo *method,int paramCount ,char** params) {
    printf("===============执行方法开始 :%s =================\n", method->getMethodName().c_str());
    JavaVFrame *javaVFrame = new JavaVFrame;//马上要执行方法了，先创建栈帧
    for (int i = 1; i <= paramCount; ++i) { //将参数写入局部变量表
        javaVFrame->locals[i] = (CommonValue *) (*(params + i - 1));
    }
    javaVFrame->locals[0] = (CommonValue *)*(params + paramCount);//非静态方法第一个参数为this指针
    javaThread->stack.push(javaVFrame);//栈帧push进线程的栈空间
    BytecodeInterpreter::run(javaThread, method); // 执行方法
    javaThread->stack.pop();//将执行完成的栈桢弹出栈空间
    delete javaVFrame; //释放栈桢内存空间
}