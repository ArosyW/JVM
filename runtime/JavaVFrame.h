//
// Created by Arosy on 2021/12/11.
//

#ifndef JVM_JAVAVFRAME_H
#define JVM_JAVAVFRAME_H
#include "stack"
#include "map"
using namespace std;

class JavaVFrame {
public:
    stack<char*> stack; // 方法的栈空间
    map<int,char*> locals; // 方法的局部变量表
};


#endif //JVM_JAVAVFRAME_H
