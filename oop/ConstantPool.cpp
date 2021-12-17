//
// Created by Arosy on 2021/11/22.
//

#include "ConstantPool.h"

/**
 * 根据JVM指令中的操作数 获取常量池中的类的全限定名
 * @param index
 * @return
 */
string ConstantPool::getClassNameByFieldInfo(unsigned short index){
    int i = htonl(*(int *)data[index]); //取出索引
    int classIndex = i >> 16; // 取出i的左16位
    //获取class全限定名
    int classNameIndex = *data[classIndex]; //取出类名的索引
    return (data[classNameIndex]); // 返回类名
};

/**
 * 根据JVM指令中的操作数 获取常量池中的类的属性名
 * @param index
 * @return
 */
string ConstantPool::getFieldName(unsigned short index){
    int i = htonl(*(int *)data[index]);//取出索引
    int ind = i & 0xFF; // 取出i的右16位
    int nameAndType = htonl(*(int *)data[ind]); // 取出nameAndType的索引
    int t = nameAndType >> 16; // 取nameAndType的索引 的左16位
    return (data[t]); // 返回变量名
};