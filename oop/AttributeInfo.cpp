//
// Created by Arosy on 2021/11/25.
//

#include "AttributeInfo.h"
short AttributeInfo::getAttributeNameIndex() const {
    return attributeNameIndex;
}

void AttributeInfo::setAttributeNameIndex(short attributeNameIndex) {
    AttributeInfo::attributeNameIndex = attributeNameIndex;
}

int AttributeInfo::getAttributeLength() const {
    return attributeLength;
}

void AttributeInfo::setAttributeLength(int attributeLength) {
    AttributeInfo::attributeLength = attributeLength;
}

void AttributeInfo::initContainer(){
    container = new char[attributeLength];
}

char *AttributeInfo::getContainer() const {
    return container;
}

void AttributeInfo::setContainer(char *container) {
    AttributeInfo::container = container;
};
