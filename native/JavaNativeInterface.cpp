//
// Created by Arosy on 2021/12/11.
//

#include "JavaNativeInterface.h"
#include "../oop/InstanceKlass.h"
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
    printf("===============执行方法 :%s =================\n", method->getMethodName().c_str());
    JavaVFrame *javaVFrame = new JavaVFrame;//马上要执行方法了，先创建栈帧
    javaThread->stack.push(javaVFrame);//栈帧push进线程的栈空间
//    BytecodeInterpreter::run(curThread, method); // 执行方法
    javaThread->stack.pop();//将执行完成的栈桢弹出栈空间
    delete javaVFrame; //释放栈桢内存空间
}