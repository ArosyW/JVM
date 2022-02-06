//
// Created by Arosy on 2022/1/15.
//

#include "CodeRunNative.h"
/**
 * 初始化本地方法
 */
map<string, CODENATIVE> CodeRunNative::map;

void CodeRunNative::initCodeRun() {
    map["write0"] = writeBytes;
}

void CodeRunNative::writeBytes(int paramsCount, char **params) {
    CommonValue *cv = (CommonValue*)*params; //目前只有一个参数 ，就直接取了
    printf("执行本地方法write0,以下为输出结果：\n");
    printf("%s\n",cv->val); //打印参数的值
};