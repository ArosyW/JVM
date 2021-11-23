//
// Created by Arosy on 2021/11/21.
//
#include "ClassFileParser.h"
#include "../util/BasicType.h"

InstanceKlass *ClassFileParser::Parser(ClassRead *classRead) {
    InstanceKlass *klass = new InstanceKlass;
    checkAndPutMagic(classRead, klass);//校验是否是class文件
    checkAndPutVersion(classRead, klass);//校验版本号
    parserConstantPoolCount(classRead, klass);//解析常量池数量
    parserConstantPool(classRead, klass);//解析常量池
    parserAccessFlags(classRead, klass);//解析访问权限
    parserThisClass(classRead, klass);//类名
    parserSuperClass(classRead, klass);//父类名
    return klass;
}

void ClassFileParser::checkAndPutMagic(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMagic(classRead->readByFourByte());
    if (klass->getMagic() == MAGIC) {
        printf("class文件校验正确\n");
        return;
    }
    printf("class文件校验错误，%X\n", klass->getMagic());
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

void ClassFileParser::parserConstantPoolCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setConstantPoolCount(classRead->readByTwoByte());
};

void ClassFileParser::parserConstantPool(ClassRead *classRead, InstanceKlass *klass) {
    klass->setConstantPool(new ConstantPool);
    ConstantPool *constantPool = klass->getConstantPool();
    constantPool->tag = new char[klass->getConstantPoolCount()];

    for (int i = 1; i < klass->getConstantPoolCount(); i++) {
        unsigned char tag = classRead->readByOneByte();
        *(constantPool->tag + i) = tag;
        switch (tag) {
            case CONSTANT_Utf8: {
                unsigned short len = classRead->readByTwoByte();
                char *target = new char[len + 1];
                classRead->readByFreeByte(len, target);
                (constantPool->data[i]) = target;
                printf("第%d个，类型utf-8，值%s\n", i, constantPool->data[i]);
                break;
            }
            case CONSTANT_Integer: {
                char *temp = new char;
                *temp = classRead->readByFourByte();
                constantPool->data[i] = temp;
                printf("第%d个，类型Integer，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_Float: {
                char *temp = new char;
                *temp = classRead->readByFourByte();
                constantPool->data[i] = temp;
                printf("第%d个，类型Float，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_Long: {
                printf("暂不处理\n");
                break;
            }
            case CONSTANT_Double: {
                printf("暂不处理\n");
                break;
            }
            case CONSTANT_Class: {
                char *temp = new char;
                *temp = classRead->readByTwoByte();
                constantPool->data[i] = temp;
                printf("第%d个，类型Class，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_String: {
                char *temp = new char[3];
                *temp = classRead->readByTwoByte();
                temp[2] = '\0';
                constantPool->data[i] = temp;
                printf("第%d个，类型String，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref: {
                int *temp = new int;
                short classIndex = classRead->readByTwoByte();
                short nameAndTypeIndex = classRead->readByTwoByte();
                *temp = htonl(classIndex << 16 | nameAndTypeIndex);
                (constantPool->data[i]) = (char *) temp;
                printf("第%d个，类型file、method、Interface Methodref，值%X\n", i, htonl(*(int *) constantPool->data[i]));
                break;
            }
            case CONSTANT_NameAndType: {
                int *temp = new int;
                short nameIndex = classRead->readByTwoByte();
                short descriptorIndex = classRead->readByTwoByte();
                *temp = htonl(nameIndex << 16 | descriptorIndex);
                (constantPool->data[i]) = (char *) temp;
                printf("第%d个，类型NameAndType，值%X\n", i, htonl(*(int *) constantPool->data[i]));
                break;
            }
            default:
                break;
        }

    }
};

void ClassFileParser::parserAccessFlags(ClassRead *classRead, InstanceKlass *klass) {
    unsigned short acc = classRead->readByTwoByte();
    klass->setAccessFlags(acc);
    printf("访问权限：%d\n", acc);
};

void ClassFileParser::parserThisClass(ClassRead *classRead, InstanceKlass *klass) {
    klass->setThisClass(classRead->readByTwoByte());
    printf("类名：%X\n", klass->getThisClass());
};

void ClassFileParser::parserSuperClass(ClassRead *classRead, InstanceKlass *klass) {
    klass->setSuperClass(classRead->readByTwoByte());
    printf("父类名：%X\n", klass->getSuperClass());
};