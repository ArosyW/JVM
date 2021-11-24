//
// Created by Arosy on 2021/11/24.
//

#ifndef JVM_FIELDSINFO_H
#define JVM_FIELDSINFO_H


class FieldsInfo {
    short accessFlag;
    short nameIndex;
    short descriptorIndex;
    short attributesCount;
public:
    short getAccessFlag() const;

    void setAccessFlag(short accessFlag);

    short getNameIndex() const;

    void setNameIndex(short nameIndex);

    short getDescriptorIndex() const;

    void setDescriptorIndex(short descriptorIndex);

    short getAttributesCount() const;

    void setAttributesCount(short attributesCount);
};


#endif //JVM_FIELDSINFO_H
