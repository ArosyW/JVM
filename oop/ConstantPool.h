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

    char* getAndNewFromPool(unsigned short index);

    string getClassPath(unsigned short index);

    string getMethodNameByMethodInfo(unsigned short index);

    string getDescriptorNameByMethodInfo(unsigned short index);

    string getClassNameByMethodInfo(unsigned short index);

};


#endif //JVM_CONSTANTPOOL_H
