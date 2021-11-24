//
// Created by Arosy on 2021/11/24.
//
#include "string"
#ifndef JVM_INTERFACESINFO_H
#define JVM_INTERFACESINFO_H

using namespace std;
class InterfacesInfo {
    short constantPoolIndex; //接口名字的常量池索引
    string interfacesName; //实现的接口的全限定名
public:
    /**
     * 构造方法
     */
    InterfacesInfo();
    InterfacesInfo(short constantPoolIndex, const string &interfacesName);
};


#endif //JVM_INTERFACESINFO_H
