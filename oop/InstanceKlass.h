//
// Created by Arosy on 2021/11/21.
//

#ifndef JVM_INSTANCEKLASS_H
#define JVM_INSTANCEKLASS_H

#include "ConstantPool.h"
#include "InterfacesInfo.h"
#include "FieldsInfo.h"
#include "MethodInfo.h"
#include "AttributeInfo.h"
#include "../runtime/CommonValue.h"

extern class InstanceOop;

class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
    short minorVersion; //JDK次版本号
    short majorVersion; //JDK主版本号
    short constantPoolCount;//常量数量
    ConstantPool *constantPool;//常量池数据
    short accessFlags;//类的访问权限
    short thisClass;//类名
    short superClass;//父类名
    short interfacesCount;//接口数量
    InterfacesInfo *interfaces;//接口
    short fieldsCount;//字段数量
    FieldsInfo *fieldsInfo;//字段
    short methodCount;//方法数量
    MethodInfo *methodInfo;//方法
    short attributeCount;//属性数量
    AttributeInfo *attributeInfo;//属性
public:
    map<string, CommonValue *> staticValue; //静态变量

/**
   * 创建对象的方法
*/
    static InstanceOop *allocateInstance(InstanceKlass *klass);

/**
   * 以下为getter && setter && 构造方法
*/

    short getAttributeCount() const;

    void setAttributeCount(short attributeCount);

    AttributeInfo *getAttributeInfo() const;

    void setAttributeInfo(AttributeInfo *attributeInfo);

    short getMethodCount() const;

    void setMethodCount(short methodCount);

    MethodInfo *getMethodInfo() const;

    void setMethodInfo(MethodInfo *methodInfo);

    short getFieldsCount() const;

    void setFieldsCount(short fieldsCount);

    FieldsInfo *getFieldsInfo() const;

    void setFieldsInfo(FieldsInfo *fieldsInfo);

    short getInterfacesCount() const;

    void setInterfacesCount(short interfacesCount);

    InterfacesInfo *getInterfaces() const;

    void setInterfaces(InterfacesInfo *interfaces);

    short getThisClass() const;

    void setThisClass(short thisClass);

    short getSuperClass() const;

    void setSuperClass(short superClass);

    short getAccessFlags() const;

    void setAccessFlags(short accessFlags);

    short getConstantPoolCount() const;

    void setConstantPoolCount(short constantPoolCount);

    ConstantPool *getConstantPool() const;

    void setConstantPool(ConstantPool *constantPool);

    short getMinorVersion() const;

    void setMinorVersion(short minorVersion);

    short getMajorVersion() const;

    void setMajorVersion(short majorVersion);

    int getMagic() const;

    void setMagic(int magic);
};


#endif //JVM_INSTANCEKLASS_H
