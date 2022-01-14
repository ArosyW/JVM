//
// Created by Arosy on 2022/1/15.
//

#include "CodeRunNative.h"
/**
 * 初始化本地方法
 */
void CodeRunNative::initCodeRun() {
    map["writeBytes"] = writeBytes;
}

void CodeRunNative::writeBytes(int paramsCount, char **params) {
    //todo 本地方法的实现
};