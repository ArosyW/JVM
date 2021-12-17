//
// Created by Arosy on 2021/12/11.
//

#include "JavaVFrame.h"

CommonValue* JavaVFrame::pop() {
    CommonValue* cv = stack.top();
    stack.pop();
    return cv;
}