//
// Created by Arosy on 2021/12/13.
//

#include "CodeRunBase.h"
#include "../oop/InstanceKlass.h"
#include "../classFile/BootClassLoader.h"
#include "ByteType.h"
#include "../util/BasicType.h"

typedef void (*CODERUN)(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index);
CODERUN CodeRunBase::run[256];
void CodeRunBase::initCodeRun(){
    run[NOP] = funcNOP; // NOP枚举值为0
    run[GETSTATIC] = funcGETSTATIC;
    run[ICONST_0] = funcICONST0;
    run[PUTSTATIC] = funcPUTSTATIC;
    run[LDC] = funcLDC;
    run[DUP] = funcDUP;
    run[ALOAD_0] = funcALOAD0;
    run[ALOAD_1] = funcALOAD1;
}
void CodeRunBase::funcNOP(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
//nothing to do .
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
void CodeRunBase::funcPUTSTATIC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令PUTSTATIC\n");
    unsigned short opera = bytecodeStream->readByTwo(index);//取出操作数
    printf("\t\tconstantPool index is:%d\n", opera);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByFieldInfo(opera);// 根据操作数 从 常量池获取类名
    string fieldName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getFieldName(opera);// 根据操作数 从 常量池获取变量名
    printf("\t\tclassName:%s,fieldName:%s\n", className.c_str(), fieldName.c_str());
    CommonValue* value =javaThread->stack.top()->pop(); //弹出栈顶变量
    InstanceKlass *klass = BootClassLoader::loadKlass(className);//根据类名取得加载完的klass对象
    klass->staticValue[fieldName] = value; //将从栈顶获得的静态变量写入类
}
void CodeRunBase::funcLDC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令LDC\n");
    unsigned char opera = bytecodeStream->readByOne(index);//取出操作数
    unsigned char tag = *(bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->tag + opera);//从常量池的类型中取出本次ldc指令操作的数据类型
    switch (tag) {
        case CONSTANT_Float:
            break;
        case CONSTANT_String: {
            int index = *bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->data[opera]; //从常量池获取索引
            char* str = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getAndNewFromPool(index);//从常量池获取string的内容
            javaThread->stack.top()->stack.push(new CommonValue(T_OBJECT, str));//入栈
            break;
        }
        case CONSTANT_Class:
            break;
        default:
            printf("未知类型\n");
    }
}
void CodeRunBase::funcICONST0(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    javaThread->stack.top()->stack.push(new CommonValue(T_INT, 0)); // 将int类型的0推向栈顶
}

void CodeRunBase::funcDUP(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令dup\n");
    CommonValue *cv = javaThread->stack.top()->stack.top(); //获取栈顶值
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}

void CodeRunBase::funcALOAD0(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令aload0\n");
    CommonValue *cv = javaThread->stack.top()->locals[0];//获取局部变量表第一个数据
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}

void CodeRunBase::funcALOAD1(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令aload1\n");
    CommonValue *cv = javaThread->stack.top()->locals[1];//获取局部变量表第一个数据
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}