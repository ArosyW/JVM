//
// Created by Arosy on 2021/11/22.
//
#include "string"
#include "map"
#ifndef JVM_CONSTANTPOOL_H
#define JVM_CONSTANTPOOL_H


class ConstantPool {
public:
    char *tag;
    std::map<int, char*> data;
};


#endif //JVM_CONSTANTPOOL_H
