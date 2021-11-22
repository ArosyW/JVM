//
// Created by Arosy on 2021/11/21.
//

#include "InstanceKlass.h"

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
