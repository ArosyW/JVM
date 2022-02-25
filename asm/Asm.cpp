//
// Created by Arosy on 2022/2/16.
//
#include <sys/mman.h>
#include <unistd.h>
#include "Asm.h"
unsigned long Asm::address = 0;
void Asm::init() {
    unsigned char * arr = static_cast<unsigned char *>(mmap(
            NULL,
            getpagesize(),
            PROT_READ | PROT_WRITE | PROT_EXEC,
            MAP_ANON | MAP_SHARED,
            0,
            0
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

void Asm::pushOrPop(int registers) {
    *(char *) address = registers;
    address++;
}

void Asm::dec(int registers) {
    *(char*)address = 0x48;
    *(char*)(address+1)=0xff;
    *(char*)(address+2)=registers;
    address += 3;
}

void Asm::CallStub() {
    *(char*)(address+0) = 0x48;
    *(char*)(address+1) = 0x85;
    *(char*)(address+2) = 0xC9;
    *(char*)(address+3) = 0x0F;
    *(char*)(address+4) = 0x84;
    *(char*)(address+5) = 0x00;
    *(char*)(address+6) = 0x00;
    *(char*)(address+7) = 0x00;
    *(char*)(address+8) = 0x00;
    *(char*)(address+9) = 0x48;
    *(char*)(address+10) = 0x31;
    *(char*)(address+11) = 0xDB;
    *(char*)(address+12) = 0x48;
    *(char*)(address+13) = 0xC1;
    *(char*)(address+14) = 0xE1;
    *(char*)(address+15) = 0x03;
    *(char*)(address+16) = 0x4C;
    *(char*)(address+17) = 0x89;
    *(char*)(address+18) = 0x0C;
    *(char*)(address+19) = 0x0C;
    *(char*)(address+20) = 0x48;
    *(char*)(address+21) = 0xC1;
    *(char*)(address+22) = 0xE9;
    *(char*)(address+23) = 0x03;
    *(char*)(address+24) = 0x48;
    *(char*)(address+25) = 0xFF;
    *(char*)(address+26) = 0xC9;
    *(char*)(address+27) = 0x49;
    *(char*)(address+28) = 0xFF;
    *(char*)(address+29) = 0xC1;
    *(char*)(address+30) = 0x75;
    *(char*)(address+31) = 0xE0;
    *(char*)(address+32) = 0x4C;
    *(char*)(address+33) = 0x89;
    *(char*)(address+34) = 0xCC;
    *(char*)(address+35) = 0x48;
    *(char*)(address+36) = 0x8B;
    *(char*)(address+37) = 0x4C;
    *(char*)(address+38) = 0x24;
    *(char*)(address+39) = 0x10;
    *(char*)(address+40) = 0x48;
    *(char*)(address+41) = 0x89;
    *(char*)(address+42) = 0xEB;
    *(char*)(address+43) = 0x67;
    *(char*)(address+44) = 0x41;
    *(char*)(address+45) = 0xFF;
    *(char*)(address+46) = 0x10;
    address += 47;
}