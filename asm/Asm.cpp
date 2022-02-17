//
// Created by Arosy on 2022/2/16.
//
#include <sys/mman.h>
#include <unistd.h>
#include "Asm.h"
unsigned long Asm::address = 0;
void Asm::init() {
    unsigned char * arr = static_cast<unsigned char *>(mmap(
            NULL,                   //分配的首地址
            getpagesize(),          //分配内存大小(必须是页的整数倍, 32位1页=4k)
            PROT_READ | PROT_WRITE | PROT_EXEC,
            MAP_ANON | MAP_SHARED,  //匿名映射(不涉及文件io), 后面两个参数忽略
            0,                      //要映射到内存中的文件描述符
            0                       //文件映射的偏移量，通常设置为0，必须是页的整数倍
    ));
    address = (unsigned long )arr;
}

unsigned long Asm::pc() {
    return address;
}

void Asm::saveCaller() {
    *(char*)address = 0x55;
    *(char*)(address+1)=0x48;
    *(char*)(address+2)=0x89;
    *(char*)(address+3)=0xe5;
    address += 4;
}

void Asm::movRcxByRsp(int offset) {
    *(char*)address = 0x48;
    *(char*)(address+1)=0x8b;
    *(char*)(address+2)=0x4c;
    *(char*)(address+3)=0x24;
    *(char*)(address+4)=offset;
    address += 5;
}

void Asm::shlRcx(int bit) {
    *(char*)address = 0x48;
    *(char*)(address+1)=0xc1;
    *(char*)(address+2)=0xe1;
    *(char*)(address+3)=bit;
    address += 4;
}

void Asm::subRspByRcx() {
    *(char*)address = 0x48;
    *(char*)(address+1)=0x29;
    *(char*)(address+2)=0xcc;
    address += 3;
}