//
// Created by Arosy on 2022/1/2.
//
#include "InstanceKlass.h"
#ifndef JVM_INSTANCEOOP_H
#define JVM_INSTANCEOOP_H


class InstanceOop {
public:
    InstanceKlass *klass;//所属类

    map<string, char *> map;//实例化的成员属性
};


#endif //JVM_INSTANCEOOP_H
