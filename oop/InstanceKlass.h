//
// Created by Arosy on 2021/11/21.
//

#ifndef JVM_INSTANCEKLASS_H
#define JVM_INSTANCEKLASS_H

#define MAGIC              0xCAFEBABE

class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
public:
    /**
     * 以下为get && set ,以及构造方法
     * @return
     */
    int getMagic() const;

    void setMagic(int magic);
};


#endif //JVM_INSTANCEKLASS_H
