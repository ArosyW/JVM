//
// Created by Arosy on 2021/11/21.
//

#ifndef JVM_CLASSFILEPARSER_H
#define JVM_CLASSFILEPARSER_H

#include "../oop/InstanceKlass.h"
#include "../Stream/ClassRead.h"
#include "../oop/LineNumberTable.h"
#include "../oop/LocalVariableTable.h"
#include "../oop/CodeAttributeInfo.h"

class ClassFileParser {
public:
    static InstanceKlass* Parser(ClassRead *classRead); // 总解析流程，里面调用小的解析流程，例如调用checkAndPutMagic
    static void checkAndPutMagic(ClassRead *classRead, InstanceKlass *klass);//验证是否是.class文件
    static void checkAndPutVersion(ClassRead *classRead, InstanceKlass *klass);//验证JDK版本号
    static void parserConstantPoolCount(ClassRead *classRead, InstanceKlass *klass);//解析常量池数量
    static void parserConstantPool(ClassRead *classRead, InstanceKlass *klass);//解析常量池数量
    static void parserAccessFlags(ClassRead *classRead, InstanceKlass *klass);//解析访问权限
    static void parserThisClass(ClassRead *classRead, InstanceKlass *klass);//解析类名
    static void parserSuperClass(ClassRead *classRead, InstanceKlass *klass);//解析父类名
    static void parserInterfacesCount(ClassRead *classRead, InstanceKlass *klass);//解析接口数量
    static void parserInterfaces(ClassRead *classRead, InstanceKlass *klass);//解析接口
    static void parserFieldsCount(ClassRead *classRead, InstanceKlass *klass);//解析字段数量
    static void parserFieldsInfo(ClassRead *classRead, InstanceKlass *klass);//解析字段
    static void parserMethodCount(ClassRead *classRead, InstanceKlass *klass);//解析方法数量
    static void parserMethodInfo(ClassRead *classRead, InstanceKlass *klass);//解析方法
    static void parserLineNumberTable(ClassRead *classRead, CodeAttributeInfo* codeAttributeInfo, string attrName, int nameIndex, InstanceKlass *klass);
    static void parseLocalVariableTable(ClassRead *classRead, CodeAttributeInfo* codeAttributeInfo, string attrName, int nameIndex, InstanceKlass *klass);


};


#endif //JVM_CLASSFILEPARSER_H
