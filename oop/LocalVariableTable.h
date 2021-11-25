//
// Created by Arosy on 2021/11/25.
//

#ifndef JVM_LOCALVARIABLETABLE_H
#define JVM_LOCALVARIABLETABLE_H


#include "AttributeInfo.h"

class LocalVariableTable : public AttributeInfo{
public:
    class Item {
        short startPc;
        short length;
        short nameIndex;
        short descriptorIndex;
        short index;
    public:
        short getStartPc() const;

        void setStartPc(short startPc);

        short getLength() const;

        void setLength(short length);

        short getNameIndex() const;

        void setNameIndex(short nameIndex);

        short getDescriptorIndex() const;

        void setDescriptorIndex(short descriptorIndex);

        short getIndex() const;

        void setIndex(short index);
    };
private:
    short tableLen;
public:
    short getTableLen() const;

    void setTableLen(short tableLen);

    Item *table;

};


#endif //JVM_LOCALVARIABLETABLE_H
