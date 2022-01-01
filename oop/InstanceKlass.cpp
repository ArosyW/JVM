//
// Created by Arosy on 2021/11/21.
//

#include "InstanceKlass.h"
#include "InstanceOop.h"

int InstanceKlass::getMagic() const {
    return magic;
}

void InstanceKlass::setMagic(int magic) {
    InstanceKlass::magic = magic;
}

short InstanceKlass::getMinorVersion() const {
    return minorVersion;
}

void InstanceKlass::setMinorVersion(short minorVersion) {
    InstanceKlass::minorVersion = minorVersion;
}

short InstanceKlass::getMajorVersion() const {
    return majorVersion;
}

void InstanceKlass::setMajorVersion(short majorVersion) {
    InstanceKlass::majorVersion = majorVersion;
}

short InstanceKlass::getConstantPoolCount() const {
    return constantPoolCount;
}

void InstanceKlass::setConstantPoolCount(short constantPoolCount) {
    InstanceKlass::constantPoolCount = constantPoolCount;
}

ConstantPool *InstanceKlass::getConstantPool() const {
    return constantPool;
}

void InstanceKlass::setConstantPool(ConstantPool *constantPool) {
    InstanceKlass::constantPool = constantPool;
}

short InstanceKlass::getAccessFlags() const {
    return accessFlags;
}

void InstanceKlass::setAccessFlags(short accessFlags) {
    InstanceKlass::accessFlags = accessFlags;
}

short InstanceKlass::getThisClass() const {
    return thisClass;
}

void InstanceKlass::setThisClass(short thisClass) {
    InstanceKlass::thisClass = thisClass;
}

short InstanceKlass::getSuperClass() const {
    return superClass;
}

void InstanceKlass::setSuperClass(short superClass) {
    InstanceKlass::superClass = superClass;
}

short InstanceKlass::getInterfacesCount() const {
    return interfacesCount;
}

void InstanceKlass::setInterfacesCount(short interfacesCount) {
    InstanceKlass::interfacesCount = interfacesCount;
}

InterfacesInfo *InstanceKlass::getInterfaces() const {
    return interfaces;
}

void InstanceKlass::setInterfaces(InterfacesInfo *interfaces) {
    InstanceKlass::interfaces = interfaces;
}

short InstanceKlass::getFieldsCount() const {
    return fieldsCount;
}

void InstanceKlass::setFieldsCount(short fieldsCount) {
    InstanceKlass::fieldsCount = fieldsCount;
}

FieldsInfo *InstanceKlass::getFieldsInfo() const {
    return fieldsInfo;
}

void InstanceKlass::setFieldsInfo(FieldsInfo *fieldsInfo) {
    InstanceKlass::fieldsInfo = fieldsInfo;
}

short InstanceKlass::getMethodCount() const {
    return methodCount;
}

void InstanceKlass::setMethodCount(short methodCount) {
    InstanceKlass::methodCount = methodCount;
}

MethodInfo *InstanceKlass::getMethodInfo() const {
    return methodInfo;
}

void InstanceKlass::setMethodInfo(MethodInfo *methodInfo) {
    InstanceKlass::methodInfo = methodInfo;
}

short InstanceKlass::getAttributeCount() const {
    return attributeCount;
}

void InstanceKlass::setAttributeCount(short attributeCount) {
    InstanceKlass::attributeCount = attributeCount;
}

AttributeInfo *InstanceKlass::getAttributeInfo() const {
    return attributeInfo;
}

void InstanceKlass::setAttributeInfo(AttributeInfo *attributeInfo) {
    InstanceKlass::attributeInfo = attributeInfo;
}

InstanceOop* InstanceKlass::allocateInstance(InstanceKlass* klass) {
    InstanceOop *oop = new InstanceOop;
    oop->klass = klass;
    //todo:对象头，申请内存等
    return oop;
}
