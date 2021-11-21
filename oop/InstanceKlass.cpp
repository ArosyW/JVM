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
