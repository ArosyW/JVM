//
// Created by Arosy on 2021/11/25.
//

#include "MethodInfo.h"
#include "InstanceKlass.h"
#include "CodeAttributeInfo.h"

void MethodInfo::setAttributeInfo(CodeAttributeInfo *attributeInfo,int index) {
    *(MethodInfo::attributeInfo + index) = *attributeInfo;
};

void MethodInfo::initCodeAttributeInfo(){
    attributeInfo = new CodeAttributeInfo[attributesCount];
}

InstanceKlass *MethodInfo::getBelongKlass() const {
    return belongKlass;
}

void MethodInfo::setBelongKlass(InstanceKlass *belongKlass) {
    MethodInfo::belongKlass = belongKlass;
}

short MethodInfo::getAccessFlags() const {
    return accessFlags;
}

void MethodInfo::setAccessFlags(short accessFlags) {
    MethodInfo::accessFlags = accessFlags;
}

int MethodInfo::getNameIndex() const {
    return nameIndex;
}

void MethodInfo::setNameIndex(int nameIndex) {
    MethodInfo::nameIndex = nameIndex;
}

int MethodInfo::getDescriptorIndex() const {
    return descriptorIndex;
}

void MethodInfo::setDescriptorIndex(int descriptorIndex) {
    MethodInfo::descriptorIndex = descriptorIndex;
}

int MethodInfo::getAttributesCount() const {
    return attributesCount;
}

void MethodInfo::setAttributesCount(int attributesCount) {
    MethodInfo::attributesCount = attributesCount;
}

CodeAttributeInfo *MethodInfo::getAttributeInfo() const {
    return attributeInfo;
}

void MethodInfo::setAttributeInfo(CodeAttributeInfo *attributeInfo) {
    MethodInfo::attributeInfo = attributeInfo;
}

const string &MethodInfo::getMethodName() const {
    return methodName;
}

void MethodInfo::setMethodName(const string &methodName) {
    MethodInfo::methodName = methodName;
}
