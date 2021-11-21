#include <iostream>
#include "Stream/ClassRead.h"


int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    printf("%X\n", classRead->readByFourByte());
    return 0;
}
