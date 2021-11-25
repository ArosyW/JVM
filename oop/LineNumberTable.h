//
// Created by Arosy on 2021/11/25.
//

#ifndef JVM_LINENUMBERTABLE_H
#define JVM_LINENUMBERTABLE_H


#include "AttributeInfo.h"

class LineNumberTable : public AttributeInfo{
public:
    class Item {
        short startPc;
        short lineNumber;
    public:
        short getStartPc() const;

        void setStartPc(short startPc);

        short getLineNumber() const;

        void setLineNumber(short lineNumber);
    };

private:
    short tableLen;
public:
    Item *table;

    void setTableLen(short tableLen);

    short getTableLen() const;

};


#endif //JVM_LINENUMBERTABLE_H
