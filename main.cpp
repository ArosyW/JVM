#include "Stream/ClassRead.h"
#include "classFile/ClassFileParser.h"
#include "oop/InstanceKlass.h"

int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    printf("字段访问权限：%d\n", klass->getFieldsInfo()->getAccessFlag());
    printf("字段名字：%s\n",klass->getConstantPool()->data[klass->getFieldsInfo()->getNameIndex()]);
    printf("字段类型：%s\n",klass->getConstantPool()->data[klass->getFieldsInfo()->getDescriptorIndex()]);
    printf("字段附加属性数量：%d\n", klass->getFieldsInfo()->getAttributesCount());
    return 0;
}