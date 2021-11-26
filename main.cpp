#include "Stream/ClassRead.h"
#include "classFile/ClassFileParser.h"
#include "oop/InstanceKlass.h"

int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    int attrIndex = klass->getAttributeInfo()->getAttributeNameIndex();
    int length = klass->getAttributeInfo()->getAttributeLength();
    short fileIndex = *(short *)(klass->getAttributeInfo()->getContainer());
    printf("解析属性完成，属性名字：%s,长度：%d，文件名字：%s\n", klass->getConstantPool()->data[attrIndex],
           length,
           klass->getConstantPool()->data[fileIndex]);
    return 0;
}