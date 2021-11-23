#include <iostream>
#include "Stream/ClassRead.h"
#include "classFile/ClassFileParser.h"
#include "oop/InstanceKlass.h"

int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    int indexClass = *(klass->getConstantPool()->data[klass->getThisClass()]);
    int indexSuperClass = *(klass->getConstantPool()->data[klass->getSuperClass()]);
    printf("类名：%s\n", klass->getConstantPool()->data[indexClass]);
    printf("父类名：%s\n", klass->getConstantPool()->data[indexSuperClass]);
    return 0;
}