//
// Created by Arosy on 2021/11/25.
//

#include "BytecodeStream.h"

MethodInfo *BytecodeStream::getBelongMethod() const {
    return belongMethod;
}

void BytecodeStream::setBelongMethod(MethodInfo *belongMethod) {
    BytecodeStream::belongMethod = belongMethod;
}

CodeAttributeInfo *BytecodeStream::getBelongCode() const {
    return belongCode;
}

void BytecodeStream::setBelongCode(CodeAttributeInfo *belongCode) {
    BytecodeStream::belongCode = belongCode;
}

int BytecodeStream::getLength() const {
    return length;
}

void BytecodeStream::setLength(int length) {
    BytecodeStream::length = length;
}

int BytecodeStream::getIndex() const {
    return index;
}

void BytecodeStream::setIndex(int index) {
    BytecodeStream::index = index;
}

char *BytecodeStream::getCodes() const {
    return codes;
}

void BytecodeStream::setCodes(char *codes) {
    BytecodeStream::codes = codes;
}

BytecodeStream::BytecodeStream(MethodInfo *belongMethod, CodeAttributeInfo *belongCode, int length, int index,
                               char *codes) : belongMethod(belongMethod), belongCode(belongCode), length(length),
                                              index(index), codes(codes) {

}

BytecodeStream::BytecodeStream() {}

