//
// Created by Arosy on 2021/11/25.
//

#include "LocalVariableTable.h"
short LocalVariableTable::getTableLen() const {
    return tableLen;
}

void LocalVariableTable::setTableLen(short tableLen) {
    LocalVariableTable::tableLen = tableLen;
}

short LocalVariableTable::Item::getStartPc() const {
    return startPc;
}

void LocalVariableTable::Item::setStartPc(short startPc) {
    Item::startPc = startPc;
}

short LocalVariableTable::Item::getLength() const {
    return length;
}

void LocalVariableTable::Item::setLength(short length) {
    Item::length = length;
}

short LocalVariableTable::Item::getNameIndex() const {
    return nameIndex;
}

void LocalVariableTable::Item::setNameIndex(short nameIndex) {
    Item::nameIndex = nameIndex;
}

short LocalVariableTable::Item::getDescriptorIndex() const {
    return descriptorIndex;
}

void LocalVariableTable::Item::setDescriptorIndex(short descriptorIndex) {
    Item::descriptorIndex = descriptorIndex;
}

short LocalVariableTable::Item::getIndex() const {
    return index;
}

void LocalVariableTable::Item::setIndex(short index) {
    Item::index = index;
}
