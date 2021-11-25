//
// Created by Arosy on 2021/11/25.
//
#include "map"
#include "string"
#include "AttributeInfo.h"
#ifndef JVM_CODEATTRIBUTEINFO_H
#define JVM_CODEATTRIBUTEINFO_H

extern class BytecodeStream;
using namespace std;
class CodeAttributeInfo {
    int attrNameIndex;
    int attrLength;
    int maxStack;
    int maxLocals;//局部变量最大个数
    BytecodeStream* code;
    int exceptionTableLength;
    int codeLength;
    int attributesCount;//局部变量表，操作数栈

public:

    map<string, AttributeInfo*> attributes;//附加属性

    int getAttrNameIndex() const;

    void setAttrNameIndex(int attrNameIndex);

    int getAttrLength() const;

    void setAttrLength(int attrLength);

    int getMaxStack() const;

    void setMaxStack(int maxStack);

    int getMaxLocals() const;

    void setMaxLocals(int maxLocals);

    BytecodeStream *getCode() const;

    void setCode(BytecodeStream *code);

    int getExceptionTableLength() const;

    void setExceptionTableLength(int exceptionTableLength);

    int getCodeLength() const;

    void setCodeLength(int codeLength);

    int getAttributesCount() const;

    void setAttributesCount(int attributesCount);

    const map<string, AttributeInfo *> &getAttributes() const;

    void setAttributes(const map<string, AttributeInfo *> &attributes);
};


#endif //JVM_CODEATTRIBUTEINFO_H
