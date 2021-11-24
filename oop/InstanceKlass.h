//
// Created by Arosy on 2021/11/21.
//

#ifndef JVM_INSTANCEKLASS_H
#define JVM_INSTANCEKLASS_H

#include "ConstantPool.h"
#include "InterfacesInfo.h"
#include "FieldsInfo.h"

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

    /**
    * 以下为getter && setter && 构造方法
    */
public:
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
