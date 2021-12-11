//
// Created by Arosy on 2021/12/11.
//

#ifndef JVM_JAVATHREAD_H
#define JVM_JAVATHREAD_H

#include "stack"
#include "JavaVFrame.h"
using namespace std;
class JavaThread {
public:
    stack<JavaVFrame*> stack; // 线程栈空间
    /**
     * 返回栈顶元素并将其弹出
     * @return
     */
    JavaVFrame* getAndPop();
};


#endif //JVM_JAVATHREAD_H
