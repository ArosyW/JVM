//
// Created by Arosy on 2021/12/17.
//

#ifndef JVM_COMMONVALUE_H
#define JVM_COMMONVALUE_H


class CommonValue {
public:
    int type ;//值的类型：int、byte、float……
    char *val;//值
    /**
     * 构造方法
     * @param type
     * @param val
     */
    CommonValue(int type, char *val);

    CommonValue();
};


#endif //JVM_COMMONVALUE_H
