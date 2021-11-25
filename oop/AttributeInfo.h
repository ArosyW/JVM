//
// Created by Arosy on 2021/11/25.
//

#ifndef JVM_ATTRIBUTEINFO_H
#define JVM_ATTRIBUTEINFO_H


class AttributeInfo {
    short attributeNameIndex;
    int attributeLength;
    char* container;

public:
    char *getContainer() const;

    void setContainer(char *container);

    char *getInfo() const;

    void setInfo(char *info);

    short getAttributeNameIndex() const;

    void setAttributeNameIndex(short attributeNameIndex);

    int getAttributeLength() const;

    void setAttributeLength(int attributeLength);

    void initContainer();

};


#endif //JVM_ATTRIBUTEINFO_H
