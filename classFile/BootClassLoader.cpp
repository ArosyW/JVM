//
// Created by Arosy on 2021/11/29.
//

#include "BootClassLoader.h"
#include "ClassFileParser.h"
#include "../native/JavaNativeInterface.h"

string BootClassLoader::prePath = "/Users/e/Documents/github/JDK/out/production/JDK/";
map<string, InstanceKlass *> BootClassLoader::allClass;

InstanceKlass *BootClassLoader::loadKlass(string& p) {
    p.append(".class");
    if (allClass[p] != 0 ) {//先判断此类是否加载过
        return allClass[p];
    }
    string tempPath = prePath;
    InstanceKlass *klass = ClassFileParser::Parser(ClassRead::readByPath(tempPath.append(p)));//读取.class到内存，然后解析它
    MethodInfo *cinit = JavaNativeInterface::getMethod(klass, "<clinit>", "()V");//找到cinit方法,执行静态变量初始化

    allClass[p] = klass;//将解析完的klass存起来
    if (cinit != NULL) {
        JavaNativeInterface::callStaticMethod(Threads::curThread,cinit);//执行cinit方法
    }
    return klass;
}