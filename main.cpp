#include "Stream/ClassRead.h"
#include "classFile/ClassFileParser.h"
#include "oop/InstanceKlass.h"
#include "classFile/BootClassLoader.h"

int main() {
    string path = "HelloJVM";
    InstanceKlass *klass = BootClassLoader::loadKlass(path);
    int index = *klass->getConstantPool()->data[klass->getThisClass()];//获取该类的类名索引
    printf("%s\n", klass->getConstantPool()->data[index]);//取出类名
    return 0;
}