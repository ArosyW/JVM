/**
 * 将.class文件读到内存中
 * created by Arosy
 */
#include <iostream>
#include "string"
#ifndef SEARCHMEMORY_CLASSREAD_H
#define SEARCHMEMORY_CLASSREAD_H

using namespace std;
class ClassRead {
    char *data;
    int cur = 0;
public:
    //构造方法
    ClassRead(char *data);

    //根据路径读取.class文件
    static ClassRead* readByPath(string path);

    unsigned char readByOneByte();//往后读取一个字节
    unsigned short  readByTwoByte();//往后读取两个字节
    unsigned int readByFourByte();//往后读取四个字节

};


#endif //SEARCHMEMORY_CLASSREAD_H
