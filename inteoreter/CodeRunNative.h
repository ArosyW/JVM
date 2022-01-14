//
// Created by Arosy on 2022/1/15.
//

#ifndef JVM_CODERUNNATIVE_H
#define JVM_CODERUNNATIVE_H

#include "map"
#include "string"
#include "../runtime/JavaVFrame.h"

typedef void (*CODENATIVE)(int paramsCount, char **params);

class CodeRunNative {
public:
    static void initCodeRun(); //JVM启动时调用，初始化本地方法

    static map<string, CODENATIVE> map; //存放本地方法
/**
 * 以下是具体的本地方法实现
 * @param paramsCount
 * @param params
 */
    static void writeBytes(int paramsCount, char **params);
};


#endif //JVM_CODERUNNATIVE_H
