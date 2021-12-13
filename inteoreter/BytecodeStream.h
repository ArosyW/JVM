//
// Created by Arosy on 2021/11/25.
//

#ifndef JVM_BYTECODESTREAM_H
#define JVM_BYTECODESTREAM_H


#include "../oop/MethodInfo.h"
#include "../oop/CodeAttributeInfo.h"

class BytecodeStream {
    MethodInfo *belongMethod;
    CodeAttributeInfo *belongCode;
    int length;
    int index;
    char *codes;

public:
    MethodInfo *getBelongMethod() const;

    void setBelongMethod(MethodInfo *belongMethod);

    CodeAttributeInfo *getBelongCode() const;

    void setBelongCode(CodeAttributeInfo *belongCode);

    int getLength() const;

    void setLength(int length);

    int getIndex() const;

    void setIndex(int index);

    char *getCodes() const;

    void setCodes(char *codes);

    BytecodeStream(MethodInfo *belongMethod, CodeAttributeInfo *belongCode, int length, int index, char *codes);

    BytecodeStream();

    unsigned char readByOne(int &offset);

    unsigned short readByTwo(int &offset);
};


#endif //JVM_BYTECODESTREAM_H
