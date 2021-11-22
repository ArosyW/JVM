//
// Created by Arosy on 2021/11/21.
//

#ifndef JVM_CLASSFILEPARSER_H
#define JVM_CLASSFILEPARSER_H

#include "../oop/InstanceKlass.h"
#include "../Stream/ClassRead.h"

class ClassFileParser {
public:
    static InstanceKlass* Parser(ClassRead *classRead); // 总解析流程，里面调用小的解析流程，例如调用checkAndPutMagic
    static void checkAndPutMagic(ClassRead *classRead, InstanceKlass *klass);//验证是否是.class文件
    static void checkAndPutVersion(ClassRead *classRead, InstanceKlass *klass);//验证JDK版本号
    static void parserConstantPoolCount(ClassRead *classRead, InstanceKlass *klass);//解析常量池数量
    static void parserConstantPool(ClassRead *classRead, InstanceKlass *klass);//解析常量池数量

};


#endif //JVM_CLASSFILEPARSER_H
