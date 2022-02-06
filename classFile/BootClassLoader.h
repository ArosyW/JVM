//
// Created by Arosy on 2021/11/29.
//
#include "map"
#include "../oop/InstanceKlass.h"
#include "../runtime/JavaThread.h"
#include "../runtime/Threads.h"

#ifndef JVM_BOOTCLASSLOADER_H
#define JVM_BOOTCLASSLOADER_H


class BootClassLoader {
public:
    static string prePath;//路径前缀 ：项目地址

    static map<string, InstanceKlass *> allClass;//所有加载完成可以使用的Java类

    static InstanceKlass* loadKlass(string& p);//根据路径（全限定名）加载Java类

};


#endif //JVM_BOOTCLASSLOADER_H
