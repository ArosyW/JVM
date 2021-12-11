//
// Created by Arosy on 2021/12/11.
//

#include "JavaThread.h"
JavaVFrame * JavaThread::getAndPop() {
    JavaVFrame *jf = stack.top();
    stack.pop();
    return jf;
}