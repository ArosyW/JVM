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
