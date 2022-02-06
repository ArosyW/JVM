//
// Created by Arosy on 2021/12/13.
//

#include "CodeRunBase.h"
#include "../oop/InstanceKlass.h"
#include "../classFile/BootClassLoader.h"
#include "ByteType.h"
#include "../util/BasicType.h"
#include "../native/JavaNativeInterface.h"
#include "CodeRunNative.h"

typedef void (*FUNNATIVE)(int paramsCount, char **params);

typedef void (*CODERUN)(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index);

CODERUN CodeRunBase::run[256];

void CodeRunBase::initCodeRun() {
    run[NOP] = funcNOP; // NOP枚举值为0
    run[DUP] = funcDUP;
    run[RETURN] = funcRETURN;
    run[GETSTATIC] = funcGETSTATIC;
    run[ICONST_0] = funcICONST0;
    run[PUTSTATIC] = funcPUTSTATIC;
    run[LDC] = funcLDC;
    run[DUP] = funcDUP;
    run[ALOAD_0] = funcALOAD0;
    run[ALOAD_1] = funcALOAD1;
    run[NEW] = funcNEW;
    run[INVOKEVIRTUAL] = funcINVOKEVIRTUAL;
    run[INVOKESPECIAL] = funcINVOKESPECIAL;
}

void CodeRunBase::funcNOP(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
//nothing to do .
}

void CodeRunBase::funcRETURN(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
//nothing to do .
    printf("    **执行指令RETURN\n");
}
void CodeRunBase::funcGETSTATIC(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    *执行指令GETSTATIC\n");
    unsigned short opera = bytecodeStream->readByTwo(index); //取出操作数
    printf("\t\tconstantPool index is:%d\n", opera);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByFieldInfo(
            opera); // 根据操作数 从 常量池获取类名
    string fieldName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getFieldName(
            opera);// 根据操作数 从 常量池获取变量名
    printf("\t\tclassName:%s,fieldName:%s\n", className.c_str(), fieldName.c_str());
    InstanceKlass *klass = BootClassLoader::loadKlass(className); //根据类名取得加载完的klass对象
    CommonValue *commonValue = klass->staticValue[fieldName]; // 根据变量名取出 值
    javaThread->stack.top()->stack.push(new CommonValue(commonValue->type, commonValue->val)); //将前面取出的值推向栈顶
}

void CodeRunBase::funcPUTSTATIC(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令PUTSTATIC\n");
    unsigned short opera = bytecodeStream->readByTwo(index);//取出操作数
    printf("\t\tconstantPool index is:%d\n", opera);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByFieldInfo(
            opera);// 根据操作数 从 常量池获取类名
    string fieldName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getFieldName(
            opera);// 根据操作数 从 常量池获取变量名
    printf("\t\tclassName:%s,fieldName:%s\n", className.c_str(), fieldName.c_str());
    CommonValue *value = javaThread->stack.top()->pop(); //弹出栈顶变量
    InstanceKlass *klass = BootClassLoader::loadKlass(className);//根据类名取得加载完的klass对象
    klass->staticValue[fieldName] = value; //将从栈顶获得的静态变量写入类
}

void CodeRunBase::funcLDC(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令LDC\n");
    unsigned char opera = bytecodeStream->readByOne(index);//取出操作数
    unsigned char tag = *(bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->tag +
                          opera);//从常量池的类型中取出本次ldc指令操作的数据类型
    switch (tag) {
        case CONSTANT_Float:
            break;
        case CONSTANT_String: {
            int index = *bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->data[opera]; //从常量池获取索引
            char *str = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getAndNewFromPool(
                    index);//从常量池获取string的内容
            javaThread->stack.top()->stack.push(new CommonValue(T_OBJECT, str));//入栈
            break;
        }
        case CONSTANT_Class:
            break;
        default:
            printf("未知类型\n");
    }
}

void CodeRunBase::funcICONST0(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    javaThread->stack.top()->stack.push(new CommonValue(T_INT, 0)); // 将int类型的0推向栈顶
}

void CodeRunBase::funcDUP(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令dup\n");
    CommonValue *cv = javaThread->stack.top()->stack.top(); //获取栈顶值
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}

void CodeRunBase::funcALOAD0(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令aload0\n");
    CommonValue *cv = javaThread->stack.top()->locals[0];//获取局部变量表第一个数据
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}

void CodeRunBase::funcALOAD1(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令aload1\n");
    CommonValue *cv = javaThread->stack.top()->locals[1];//获取局部变量表第一个数据
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}

void CodeRunBase::funcNEW(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令NEW\n");
    unsigned short opera = bytecodeStream->readByTwo(index);
    string classPath = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassPath(opera);
    InstanceKlass *klass = BootClassLoader::loadKlass(classPath);
    InstanceOop *oop = InstanceKlass::allocateInstance(klass);
    javaThread->stack.top()->stack.push(new CommonValue(T_NARROWOOP, (char *) oop));
}

void CodeRunBase::funcINVOKEVIRTUAL(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令INVOKEVIRTUAL\n");
    unsigned short opera = bytecodeStream->readByTwo(index);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByMethodInfo(opera);//获取类名
    string methodName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getMethodNameByMethodInfo(opera);//获取方法名
    string descName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getDescriptorNameByMethodInfo(opera);//获取方法描述
    printf("\tclassName:%s,methodName:%s,descName:%s\n", className.c_str(), methodName.c_str(),
           descName.c_str());
    int paramCount =0 ;//初始化参数数量
    char **params = CodeRunBase::getParams(descName, javaThread->stack.top(),paramCount);//解析参数
    InstanceKlass *klass = BootClassLoader::loadKlass(className);//获取类全限定名
    MethodInfo *m = JavaNativeInterface::getMethod(klass, methodName, descName);//根据方法名字和方法描述找到要调用的方法
    JavaNativeInterface::callVirtual(javaThread, m, paramCount, params);//调用方法
}

void CodeRunBase::funcINVOKESPECIAL(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令INVOKESPECIAL\n");
    unsigned short opera = bytecodeStream->readByTwo(index);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByMethodInfo(opera);//获取类名
    string methodName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getMethodNameByMethodInfo(opera);//获取方法名
    string descName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getDescriptorNameByMethodInfo(opera);//获取方法描述
    printf("\tclassName:%s,methodName:%s,descName:%s\n", className.c_str(), methodName.c_str(),
           descName.c_str());
    int paramCount =0 ;//初始化参数数量
    char **params = CodeRunBase::getParams(descName, javaThread->stack.top(),paramCount);//解析参数
    InstanceKlass *klass = BootClassLoader::loadKlass(className);//获取类全限定名
    MethodInfo *m = JavaNativeInterface::getMethod(klass, methodName, descName);//根据方法名字和方法描述找到要调用的方法
    if ((m->getAccessFlags() & 100000000) == 0x0100) {//判断是否是本地方法
        FUNNATIVE nativeFunc = (FUNNATIVE) CodeRunNative::map[m->getMethodName()]; //取出本地方法
        nativeFunc(paramCount,params);//调用本地方法
        return;
    }
    JavaNativeInterface::callSpecial(javaThread, m, paramCount, params);//调用方法
}

char** CodeRunBase::getParams(string descriptor, JavaVFrame *jf,int & paramCount) {
    int start = descriptor.find("(");
    int end = descriptor.find(")");
    string paramStr = descriptor.substr(start + 1, end - 1);
    char *split = std::strtok(const_cast<char *>(paramStr.data()), ";");
    while (split != NULL) {
        paramCount++;
        split = std::strtok(NULL, ";");
    }
    char **res = reinterpret_cast<char **>(new char[paramCount+1]);//非静态方法的第一个参数用于是this指针，因此多申请一个内存
    for (int i = 0; i <= paramCount; i++) {
        *(res + i) = (char *) jf->pop();
    }
    return res;
}