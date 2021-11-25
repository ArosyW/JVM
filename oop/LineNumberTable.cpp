//
// Created by Arosy on 2021/11/25.
//

#include "LineNumberTable.h"
short LineNumberTable::Item::getStartPc() const {
    return startPc;
}

void LineNumberTable::Item::setStartPc(short startPc) {
    Item::startPc = startPc;
}

short LineNumberTable::Item::getLineNumber() const {
    return lineNumber;
}

void LineNumberTable::Item::setLineNumber(short lineNumber) {
    Item::lineNumber = lineNumber;
}

void LineNumberTable::setTableLen(short tableLen) {
    LineNumberTable::tableLen = tableLen;
}

short LineNumberTable::getTableLen() const {
    return tableLen;
}
