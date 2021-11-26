//
// Created by Arosy on 2021/11/21.
//
#include "ClassFileParser.h"
#include "../util/BasicType.h"
#include "../inteoreter/BytecodeStream.h"

InstanceKlass *ClassFileParser::Parser(ClassRead *classRead) {
    InstanceKlass *klass = new InstanceKlass;
    checkAndPutMagic(classRead, klass);//校验是否是class文件
    checkAndPutVersion(classRead, klass);//校验版本号
    parserConstantPoolCount(classRead, klass);//解析常量池数量
    parserConstantPool(classRead, klass);//解析常量池
    parserAccessFlags(classRead, klass);//解析访问权限
    parserThisClass(classRead, klass);//类名
    parserSuperClass(classRead, klass);//父类名
    parserInterfacesCount(classRead, klass);//解析出接口数量
    parserInterfaces(classRead, klass);//根据数量解析接口
    parserFieldsCount(classRead, klass);//解析字段数量
    parserFieldsInfo(classRead, klass);//根据数量解析字段
    parserMethodCount(classRead, klass);//解析方法数量
    parserMethodInfo(classRead, klass);//解析方法
    parserAttributeCount(classRead, klass);//解析属性数量
    parserAttribute(classRead, klass);//解析属性
    return klass;
}

void ClassFileParser::checkAndPutMagic(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMagic(classRead->readByFourByte());//读取4个字节并set进InstanceKlass的magic属性中
    if (klass->getMagic() == MAGIC) {//校验magic属性是否等于0xCAFEBABE
        printf("class文件校验正确\n");
        return;
    }
    printf("class文件校验错误，%X\n", klass->getMagic());
}

void ClassFileParser::checkAndPutVersion(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMinorVersion(classRead->readByTwoByte());//读取两个字节作为次版本号set进InstanceKlass属性
    klass->setMajorVersion(classRead->readByTwoByte());//读取两个字节作为主版本号set进InstanceKlass属性
    printf("次版本号：%d\n", klass->getMinorVersion());
    printf("主版本号：%d\n", klass->getMajorVersion());
    switch (klass->getMajorVersion()) {//校验版本号是否在允许范围内
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
    klass->setConstantPoolCount(classRead->readByTwoByte());//读取两个字节获取到常量池的数量，并set进InstanceKlass属性
};

void ClassFileParser::parserConstantPool(ClassRead *classRead, InstanceKlass *klass) {
    klass->setConstantPool(new ConstantPool);//为常量池初始化内存空间
    ConstantPool *constantPool = klass->getConstantPool();
    constantPool->tag = new char[klass->getConstantPoolCount()];//根据常量池数量初始化常量池数据区的内存空间

    for (int i = 1; i < klass->getConstantPoolCount(); i++) {
        unsigned char tag = classRead->readByOneByte();//读取一个字节获取到类型的映射值
        *(constantPool->tag + i) = tag;//存储类型
        switch (tag) {//根据不同的类型，有不同的处理方式，主要是读取的字节数不同
            case CONSTANT_Utf8: {
                unsigned short len = classRead->readByTwoByte();//读两个字节作为utf-8的字节长度
                char *target = new char[len + 1]; //申请len+1长度的字节内存，c语言最后一位需要用'\0'填充
                classRead->readByFreeByte(len, target);//读取len个字节
                (constantPool->data[i]) = target;//存储在常量池
                printf("第%d个，类型utf-8，值%s\n", i, constantPool->data[i]);
                break;
            }
            case CONSTANT_Integer: {
                char *temp = new char;
                *temp = classRead->readByFourByte(); //读取四个字节
                constantPool->data[i] = temp;//存储在常量池
                printf("第%d个，类型Integer，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_Float: {
                char *temp = new char;
                *temp = classRead->readByFourByte();//读取四个字节
                constantPool->data[i] = temp;//存储在常量池
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
                *temp = classRead->readByTwoByte();//读取两个字节
                constantPool->data[i] = temp;//存储在常量池
                printf("第%d个，类型Class，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_String: {
                char *temp = new char[3];
                *temp = classRead->readByTwoByte();//读取两个字节
                temp[2] = '\0';
                constantPool->data[i] = temp;//存储在常量池
                printf("第%d个，类型String，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref: {
                int *temp = new int;//申请四个字节的内存空间
                short classIndex = classRead->readByTwoByte();//读取两个字节
                short nameAndTypeIndex = classRead->readByTwoByte();//读取两个字节
                *temp = htonl(classIndex << 16 | nameAndTypeIndex);//左16位存储classIndex 右16为存储nameAndTypeIndex
                (constantPool->data[i]) = (char *) temp;//存储在常量池
                printf("第%d个，类型file、method、Interface Methodref，值%X\n", i, htonl(*(int *) constantPool->data[i]));
                break;
            }
            case CONSTANT_NameAndType: {
                int *temp = new int;
                short nameIndex = classRead->readByTwoByte();//读取两个字节
                short descriptorIndex = classRead->readByTwoByte();//读取两个字节
                *temp = htonl(nameIndex << 16 | descriptorIndex);//左16位存储classIndex 右16为存储nameAndTypeIndex
                (constantPool->data[i]) = (char *) temp;//存储在常量池
                printf("第%d个，类型NameAndType，值%X\n", i, htonl(*(int *) constantPool->data[i]));
                break;
            }
            default:
                break;
        }

    }
};

void ClassFileParser::parserAccessFlags(ClassRead *classRead, InstanceKlass *klass) {
    klass->setAccessFlags(classRead->readByTwoByte());//读取两个字节，set进InstanceKlass属性
    printf("访问权限：%d\n", klass->getAccessFlags());
};

void ClassFileParser::parserThisClass(ClassRead *classRead, InstanceKlass *klass) {
    klass->setThisClass(classRead->readByTwoByte());//读取两个字节，set进InstanceKlass属性
    printf("类名：%X\n", klass->getThisClass());
};

void ClassFileParser::parserSuperClass(ClassRead *classRead, InstanceKlass *klass) {
    klass->setSuperClass(classRead->readByTwoByte());//读取两个字节，set进InstanceKlass属性
    printf("父类名：%X\n", klass->getSuperClass());
};

void ClassFileParser::parserInterfacesCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setInterfacesCount(classRead->readByTwoByte());//往后读取两个字节set进InstanceKlass中的接口数量属性
    printf("接口数量：%d\n", klass->getInterfacesCount());
}

void ClassFileParser::parserInterfaces(ClassRead *classRead, InstanceKlass *klass) {
    if (klass->getInterfacesCount() == 0) {
        return;//未实现接口就不解析了
    }
    klass->setInterfaces(new InterfacesInfo[klass->getInterfacesCount()]);
    InterfacesInfo *interfaces = klass->getInterfaces();
    for (int i = 0; i < klass->getInterfacesCount(); i++) {
        unsigned short constantPoolIndex = classRead->readByTwoByte();//往后读取两个字节作为常量池索引
        int index = *(klass->getConstantPool()->data[constantPoolIndex]);//获取新的常量池索引
        string name = klass->getConstantPool()->data[index];//第二次索引取得全限定名
        printf("第%d个接口，name:%s,索引位置：%d", i + 1, name.c_str(), constantPoolIndex);
        *(interfaces + i) = *(new InterfacesInfo(constantPoolIndex, name));//将接口set进InstanceKlass属性中
    }
};

void ClassFileParser::parserFieldsCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setFieldsCount(classRead->readByTwoByte());//往后读取两个字节set进InstanceKlass中的字段数量属性
    printf("字段数量：%d\n", klass->getFieldsCount());
}

void ClassFileParser::parserFieldsInfo(ClassRead *classRead, InstanceKlass *klass) {
    klass->setFieldsInfo(new FieldsInfo[klass->getFieldsCount()]);//按照字段数量初始化字段内存空间
    FieldsInfo *fieldsInfo = klass->getFieldsInfo();
    for (int i = 0; i < klass->getFieldsCount(); i++) {//循环字段数量count次，解析字段
        FieldsInfo *fields = (fieldsInfo + i);
        fields->setAccessFlag(classRead->readByTwoByte());//往后读取两个字节set进FieldsInfo的访问权限
        fields->setNameIndex(classRead->readByTwoByte());//往后读取两个字节set进FieldsInfo的name
        fields->setDescriptorIndex(classRead->readByTwoByte());//往后读取两个字节set进FieldsInfo的类型描述
        fields->setAttributesCount(classRead->readByTwoByte());//往后读取两个字节set进FieldsInfo的附加属性数量
        printf("field 解析：\n access:%X,\n nameIndex:%X\n descriptorIndex:%X \n attributesCount:%X\n",
               fields->getAccessFlag(), fields->getNameIndex(), fields->getDescriptorIndex(),
               fields->getAttributesCount());
    }
};

void ClassFileParser::parserMethodCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMethodCount(classRead->readByTwoByte());
    printf("方法数量：%d\n", klass->getMethodCount());
};

void ClassFileParser::parserMethodInfo(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMethodInfo(new MethodInfo[klass->getMethodCount()]);//初始化InstanceKlass中的methodInfo的内存空间
    for (int i = 0; i < klass->getMethodCount(); i++) {
        MethodInfo *method = new MethodInfo;
        method->setBelongKlass(klass);//将方法与所属InstanceKlass关联
        method->setAccessFlags(classRead->readByTwoByte());//存储方法的访问权限
        method->setNameIndex(classRead->readByTwoByte());//存储方法的访问权限
        method->setMethodName((string) klass->getConstantPool()->data[method->getNameIndex()]);//存储方法的名字
        printf("解析方法%s\n", method->getMethodName().c_str());
        method->setDescriptorIndex(classRead->readByTwoByte());//存储方法的描述（包含了参数、返回值）
        method->setAttributesCount(classRead->readByTwoByte());//存储属性数量
        method->initCodeAttributeInfo();
        *(klass->getMethodInfo() + i) = *method;
        for (int j = 0; j < method->getAttributesCount(); j++) {
            CodeAttributeInfo *codeAttributeInfo = new CodeAttributeInfo;
            codeAttributeInfo->setAttrNameIndex(classRead->readByTwoByte());//存储属性名字的常量池索引
            codeAttributeInfo->setAttrLength(classRead->readByFourByte());//存储属性长度
            codeAttributeInfo->setMaxStack(classRead->readByTwoByte());//存储最大栈深度
            codeAttributeInfo->setMaxLocals(classRead->readByTwoByte());//存储局部变量表数量
            codeAttributeInfo->setCodeLength(classRead->readByFourByte());//存储指令数量
            BytecodeStream *bytecodeStream = new BytecodeStream(method, codeAttributeInfo,
                                                                codeAttributeInfo->getCodeLength(), 0,
                                                                new char[codeAttributeInfo->getCodeLength()]);
            classRead->readByFreeByte(codeAttributeInfo->getCodeLength(), bytecodeStream->getCodes()); //重点： 将方法的JVM指令存储在bytecodeStream
            codeAttributeInfo->setCode(bytecodeStream);
            printf("\t第%d个属性，access flag:%X name index : %X  stack:%X container:%X  code length:%X \n", j,
                   method->getAccessFlags(), codeAttributeInfo->getAttrNameIndex(), codeAttributeInfo->getMaxStack(),
                   codeAttributeInfo->getMaxLocals(), codeAttributeInfo->getCodeLength());
            codeAttributeInfo->setExceptionTableLength(classRead->readByTwoByte());//存储Exception表长度，此处暂时为0，因为我们没有任何异常需要处理
            codeAttributeInfo->setAttributesCount(classRead->readByTwoByte());//存储属性长度
            method->setAttributeInfo(codeAttributeInfo, j);
            for (int k = 0; k < codeAttributeInfo->getAttributesCount(); k++) {//循环解析属性
                int nameIndex = classRead->readByTwoByte();
                string attrName = klass->getConstantPool()->data[nameIndex];
                if ("LineNumberTable" == attrName) {//解析LineNumberTable
                    parserLineNumberTable(classRead, codeAttributeInfo, attrName, nameIndex, klass);
                } else if ("LocalVariableTable" == attrName) {//解析LocalVariableTable
                    parseLocalVariableTable(classRead, codeAttributeInfo, attrName, nameIndex, klass);
                }
            }
        }
    }
};


void ClassFileParser::parserLineNumberTable(ClassRead *classRead, CodeAttributeInfo *codeAttributeInfo, string attrName,
                                            int nameIndex, InstanceKlass *klass) {
    LineNumberTable *lineNumberTable = new LineNumberTable;
    (codeAttributeInfo->attributes)[attrName] = lineNumberTable;
    lineNumberTable->setAttributeNameIndex(nameIndex);
    lineNumberTable->setAttributeLength(classRead->readByFourByte());
    lineNumberTable->setTableLen(classRead->readByTwoByte());
    lineNumberTable->table = new LineNumberTable::Item[lineNumberTable->getTableLen()];
    if (lineNumberTable->getTableLen() == 0) {
        printf("table len =0\n");
        return ;
    }
    for (int i = 0; i < lineNumberTable->getTableLen(); i++) {
        LineNumberTable::Item *item = new LineNumberTable::Item;
        *(lineNumberTable->table + i) = *item;
        printf("\t\tlineNumberTable: name index:%d,attr len:%d, table len:%d\n",
               lineNumberTable->getAttributeNameIndex(),
               lineNumberTable->getAttributeLength(), lineNumberTable->getTableLen());
        item->setStartPc(classRead->readByTwoByte());
        item->setLineNumber(classRead->readByTwoByte());
        printf("\t\t\t第%d个属性，start pc : %d,line numnber:%d\n", i, item->getStartPc(), item->getLineNumber());
    }

};

void ClassFileParser::parseLocalVariableTable(ClassRead *classRead, CodeAttributeInfo *codeAttributeInfo, string attrName,
                                         int nameIndex, InstanceKlass *klass) {
    LocalVariableTable *localVariableTable = new LocalVariableTable;
    (codeAttributeInfo->attributes)[attrName] = localVariableTable;
    localVariableTable->setAttributeNameIndex(nameIndex);
    localVariableTable->setAttributeLength(classRead->readByFourByte());
    localVariableTable->setTableLen(classRead->readByTwoByte());
    localVariableTable->table = new LocalVariableTable::Item[localVariableTable->getTableLen()];
    if (localVariableTable->getTableLen() == 0) {
        printf("table len =0\n");
        return;
    }
    for (int i = 0; i < localVariableTable->getTableLen(); i++) {
        LocalVariableTable::Item *item = new LocalVariableTable::Item;
        *(localVariableTable->table + i) = *item;
        item->setStartPc(classRead->readByTwoByte());
        item->setLength(classRead->readByTwoByte());
        item->setNameIndex(classRead->readByTwoByte());
        item->setDescriptorIndex(classRead->readByTwoByte());
        item->setIndex(classRead->readByTwoByte());
        printf("\t\tLocalVariableTable:第%d个属性，start pc:%d,length:%d,name index:%d,descrip:%d,index:%d\n", i,
               item->getStartPc(), item->getLength(),
               item->getNameIndex(), item->getDescriptorIndex(), item->getIndex());
    }

};

void ClassFileParser::parserAttributeCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setAttributeCount(classRead->readByTwoByte());//读取两个字节将属性数量存储在InstanceKlass中
    printf("解析属性，数量%d\n", klass->getAttributeCount());
};

void ClassFileParser::parserAttribute(ClassRead *classRead, InstanceKlass *klass) {
    for (int i = 0; i < klass->getAttributeCount(); i++) {
        short nameIndex = classRead->readByTwoByte();
        string attrName = klass->getConstantPool()->data[nameIndex];
        if ("SourceFile" == attrName) {
            printf("\tSourceFile\n");
            parseSourceFile(classRead, klass, nameIndex, i);//解析SourceFile属性
        } else {
            printf("\t无法识别的属性:%X\n", attrName.c_str());
        }
    }
};

void ClassFileParser::parseSourceFile(ClassRead *classRead, InstanceKlass *klass, short nameIndex, short index) {
    klass->setAttributeInfo(new AttributeInfo[klass->getAttributeCount()]);//初始化内存空间来存储属性
    AttributeInfo *attributeInfo= klass->getAttributeInfo();
    *(klass->getAttributeInfo() + index) = *attributeInfo;
    attributeInfo->setAttributeNameIndex(nameIndex);//存储属性name的常量池索引
    attributeInfo->setAttributeLength(classRead->readByFourByte());//存储属性长度length
    attributeInfo->initContainer();
    *(attributeInfo->getContainer()) = classRead->readByTwoByte();//存储文件名的常量池索引
    printf("\t\t第%d个属性，%s:nameIndex:%d,length:%d,data:%d,(%s)\n", index, klass->getConstantPool()->data[nameIndex],
           attributeInfo->getAttributeNameIndex(), attributeInfo->getAttributeLength(), *attributeInfo->getContainer(),
           klass->getConstantPool()->data[*attributeInfo->getContainer()]);
};


