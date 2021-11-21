#include "ClassRead.h"
#include <iostream>
/**
 * 所用到的 htonl() 函数 为大端与小端转换 ，无需深究
 */

/**
 * 根据路径读取.class文件
 */
ClassRead* ClassRead::readByPath(string path){
    FILE *fp = fopen(path.c_str(), "r");//c++ 流的操作
    fseek(fp, 0, SEEK_END);
    char *c = new char[ftell(fp)];  // ftell(fp)函数可获取此文件字节个数 ， 然后申请count个字节的内存空间
    int cur = 0;
    fseek(fp, 0, 0);
    //下面将流中的字节复制到自己申请的内存中
    while (1) {
        unsigned char s = fgetc(fp);
        *(c + cur) = s;
        cur++;
        if (feof(fp)) {
            break;
        }
    }
    return new ClassRead(c);//返回一个ClassRead对象
}

/**
 * 往后读取一个字节
 */
unsigned char   ClassRead::readByOneByte(){
    unsigned char *res = (unsigned char *) (data+cur);
    cur++;
    return (*res);
}
/**
 * 往后读取2个字节
 */
unsigned short ClassRead::readByTwoByte(){
    unsigned short *res = (unsigned short *) (data+cur);
    cur = cur + 2;
    return htons(*res);
}
/**
 * 往后读取4个字节
 */
unsigned int  ClassRead::readByFourByte(){
    int *res = (int *) (data+cur);
    cur = cur + 4;
    return htonl(*res);
}

ClassRead::ClassRead(char *data) : data(data) {}//构造方法
