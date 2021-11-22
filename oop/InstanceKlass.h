//
// Created by Arosy on 2021/11/21.
//

#ifndef JVM_INSTANCEKLASS_H
#define JVM_INSTANCEKLASS_H

#define MAGIC              0xCAFEBABE

class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
    short minorVersion; //JDK次版本号
    short majorVersion; //JDK主版本号
public:
/**
 * 以下为getter && setter && 构造方法
 */
    short getMinorVersion() const;

    void setMinorVersion(short minorVersion);

    short getMajorVersion() const;

    void setMajorVersion(short majorVersion);

    int getMagic() const;

    void setMagic(int magic);
};


#endif //JVM_INSTANCEKLASS_H
