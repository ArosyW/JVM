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
//    BytecodeInterpreter::run(curThread, method); // 执行方法
    JavaVFrame *javaVFrame = javaThread->getAndPop(); // 取值当前线程栈顶的栈帧
    delete javaVFrame; //释放栈桢内存空间
}