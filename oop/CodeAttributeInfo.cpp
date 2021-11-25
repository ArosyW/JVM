//
// Created by Arosy on 2021/11/25.
//

#include "CodeAttributeInfo.h"

int CodeAttributeInfo::getAttrNameIndex() const {
    return attrNameIndex;
}

void CodeAttributeInfo::setAttrNameIndex(int attrNameIndex) {
    CodeAttributeInfo::attrNameIndex = attrNameIndex;
}

int CodeAttributeInfo::getAttrLength() const {
    return attrLength;
}

void CodeAttributeInfo::setAttrLength(int attrLength) {
    CodeAttributeInfo::attrLength = attrLength;
}

int CodeAttributeInfo::getMaxStack() const {
    return maxStack;
}

void CodeAttributeInfo::setMaxStack(int maxStack) {
    CodeAttributeInfo::maxStack = maxStack;
}

int CodeAttributeInfo::getMaxLocals() const {
    return maxLocals;
}

void CodeAttributeInfo::setMaxLocals(int maxLocals) {
    CodeAttributeInfo::maxLocals = maxLocals;
}

BytecodeStream *CodeAttributeInfo::getCode() const {
    return code;
}

void CodeAttributeInfo::setCode(BytecodeStream *code) {
    CodeAttributeInfo::code = code;
}

int CodeAttributeInfo::getExceptionTableLength() const {
    return exceptionTableLength;
}

void CodeAttributeInfo::setExceptionTableLength(int exceptionTableLength) {
    CodeAttributeInfo::exceptionTableLength = exceptionTableLength;
}

int CodeAttributeInfo::getCodeLength() const {
    return codeLength;
}

void CodeAttributeInfo::setCodeLength(int codeLength) {
    CodeAttributeInfo::codeLength = codeLength;
}

int CodeAttributeInfo::getAttributesCount() const {
    return attributesCount;
}

void CodeAttributeInfo::setAttributesCount(int attributesCount) {
    CodeAttributeInfo::attributesCount = attributesCount;
}

const map<string, AttributeInfo *> &CodeAttributeInfo::getAttributes() const {
    return attributes;
}

void CodeAttributeInfo::setAttributes(const map<string, AttributeInfo *> &attributes) {
    CodeAttributeInfo::attributes = attributes;
}
