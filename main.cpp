#include "Stream/ClassRead.h"
#include "classFile/ClassFileParser.h"
#include "oop/InstanceKlass.h"

int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/qsc/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    return 0;
}