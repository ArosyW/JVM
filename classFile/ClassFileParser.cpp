//
// Created by Arosy on 2021/11/21.
//
#include "ClassFileParser.h"
InstanceKlass *ClassFileParser::Parser(ClassRead *classRead) {
    InstanceKlass *klass = new InstanceKlass;
    checkAndPutMagic(classRead, klass);//校验是否是class文件
    checkAndPutVersion(classRead, klass);//校验版本号
    return klass;
}

void ClassFileParser::checkAndPutVersion(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMinorVersion(classRead->readByTwoByte());
    klass->setMajorVersion(classRead->readByTwoByte());
    printf("次版本号：%d\n", klass->getMinorVersion());
    printf("主版本号：%d\n", klass->getMajorVersion());
    switch (klass->getMajorVersion()) {
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52: {
            if (klass->getMinorVersion() == 0) {
                printf("版本校验正确\n");
                return;
            }
        }
    }
    printf("版本校验错误\n");
};

void ClassFileParser::checkAndPutMagic(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMagic(classRead->readByFourByte());
    if (klass->getMagic() == MAGIC) {
        printf("class文件校验正确\n");
        return;
    }
    printf("class文件校验错误，%X\n", klass->getMagic());
}
