#include "ClassRead.h"
#include <iostream>
/**
 * 所用到的 htonl() 函数 为大端与小端转换 ，无需深究
 */

/**
 * 根据路径读取.class文件
 */
ClassRead* ClassRead::readByPath(string path){
    FILE *fp = fopen(path.c_str(), "r");
    fseek(fp, 0, SEEK_END);
    ftell(fp);
    char *c = new char[ftell(fp)];
    int cur = 0;
    fseek(fp, 0, 0);
    while (1) {
        unsigned char s = fgetc(fp);
        *(c + cur) = s;
        cur++;
        if (feof(fp)) {
            break;
        }
    }
    return new ClassRead(c);
}

/**
 * 往前读取一个字节
 */
unsigned char   ClassRead::readByOneByte(){
    unsigned char *res = (unsigned char *) (data+cur);
    cur++;
    return (*res);
}
/**
 * 往前读取2个字节
 */
unsigned short ClassRead::readByTwoByte(){
    unsigned short *res = (unsigned short *) (data+cur);
    cur = cur + 2;
    return htons(*res);
}
/**
 * 往前读取4个字节
 */
unsigned int  ClassRead::readByFourByte(){
    int *res = (int *) (data+cur);
    cur = cur + 4;
    return htonl(*res);
}

ClassRead::ClassRead(char *data) : data(data) {}//构造方法
