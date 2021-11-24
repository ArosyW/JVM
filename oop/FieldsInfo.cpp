//
// Created by Arosy on 2021/11/24.
//

#include "FieldsInfo.h"

short FieldsInfo::getAccessFlag() const {
    return accessFlag;
}

void FieldsInfo::setAccessFlag(short accessFlag) {
    FieldsInfo::accessFlag = accessFlag;
}

short FieldsInfo::getNameIndex() const {
    return nameIndex;
}

void FieldsInfo::setNameIndex(short nameIndex) {
    FieldsInfo::nameIndex = nameIndex;
}

short FieldsInfo::getDescriptorIndex() const {
    return descriptorIndex;
}

void FieldsInfo::setDescriptorIndex(short descriptorIndex) {
    FieldsInfo::descriptorIndex = descriptorIndex;
}

short FieldsInfo::getAttributesCount() const {
    return attributesCount;
}

void FieldsInfo::setAttributesCount(short attributesCount) {
    FieldsInfo::attributesCount = attributesCount;
}
