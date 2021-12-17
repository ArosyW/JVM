//
// Created by Arosy on 2021/11/22.
//
#include "string"
#include "map"
#include "string"
#ifndef JVM_CONSTANTPOOL_H
#define JVM_CONSTANTPOOL_H

using namespace std;
class ConstantPool {
public:
    char *tag;
    std::map<int, char*> data;
    
    string getClassNameByFieldInfo(unsigned short index);

    string getFieldName(unsigned short index);

    string getStringFromPool(unsigned short index);

};


#endif //JVM_CONSTANTPOOL_H
