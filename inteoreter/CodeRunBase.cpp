//
// Created by Arosy on 2021/12/13.
//

#include "CodeRunBase.h"
#include "../oop/InstanceKlass.h"
#include "../classFile/BootClassLoader.h"
typedef void (*CODERUN)(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
CODERUN CodeRunBase::run[256];
void CodeRunBase::initCodeRun(){
    run[NOP] = funcNOP; // NOP枚举值为0
    run[GETSTATIC] = funcGETSTATIC;
}
void CodeRunBase::funcNOP(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
//nothing to do
}
void CodeRunBase::funcGETSTATIC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    *执行指令GETSTATIC\n");
    unsigned short opera = bytecodeStream->readByTwo(index); //取出操作数
    printf("\t\tconstantPool index is:%d\n", opera);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByFieldInfo(opera); // 根据操作数 从 常量池获取类名
    string fieldName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getFieldName(opera);// 根据操作数 从 常量池获取变量名
    printf("\t\tclassName:%s,fieldName:%s\n", className.c_str(), fieldName.c_str());
    InstanceKlass *klass = BootClassLoader::loadKlass(className); //根据类名取得加载完的klass对象
    CommonValue *commonValue = klass->staticValue[fieldName]; // 根据变量名取出 值
    javaThread->stack.top()->stack.push(new CommonValue(commonValue->type, commonValue->val)); //将前面取出的值推向栈顶
}