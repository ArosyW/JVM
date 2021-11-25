//
// Created by Arosy on 2021/11/25.
//
#include "string"
#ifndef JVM_METHODINFO_H
#define JVM_METHODINFO_H
using namespace std;
extern class CodeAttributeInfo;
extern class InstanceKlass ;
class MethodInfo {
    InstanceKlass* belongKlass;
    short accessFlags;
    int  nameIndex;
    int descriptorIndex;
    int attributesCount;
    CodeAttributeInfo* attributeInfo;
    string methodName;
public:
    void setAttributeInfo(CodeAttributeInfo *attributeInfo,int index);

    void initCodeAttributeInfo();

    InstanceKlass *getBelongKlass() const;

    void setBelongKlass(InstanceKlass *belongKlass);

    short getAccessFlags() const;

    void setAccessFlags(short accessFlags);

    int getNameIndex() const;

    void setNameIndex(int nameIndex);

    int getDescriptorIndex() const;

    void setDescriptorIndex(int descriptorIndex);

    int getAttributesCount() const;

    void setAttributesCount(int attributesCount);

    CodeAttributeInfo *getAttributeInfo() const;

    void setAttributeInfo(CodeAttributeInfo *attributeInfo);

    const string &getMethodName() const;

    void setMethodName(const string &methodName);
};


#endif //JVM_METHODINFO_H
