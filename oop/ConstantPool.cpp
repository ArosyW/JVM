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
/**
 * 根据JVM指令中的操作数 获取常量池中的一个字符串
 * @param index
 * @return
 */
string ConstantPool::getStringFromPool(unsigned short index){
    return  (data[index]);
};
/**
 * 根据JVM指令中的操作数 获取常量池中的一个字符串 并且 重新在堆中分配内存（复制一份）
 * @param index
 * @return
 */
char* ConstantPool::getAndNewFromPool(unsigned short index){
    string temp = getStringFromPool(index);
    char *res = new char[temp.length()];
    temp.copy(res, temp.length(), 0);
    res[temp.length()] = '\0';
    return  (data[index]);
};
/**
 * 根据JVM指令中的操作数 获取常量池中类的全限定名
 * @param index
 * @return
 */
string ConstantPool::getClassPath(unsigned short index){
    int ind = *(data[index]);
    return data[ind];
};
/**
 * 根据JVM指令中的操作数 获取常量池中方法的名字
 * @param index
 * @return
 */
string ConstantPool::getMethodNameByMethodInfo(unsigned short index){
    int i = htonl(*(int *)data[index]);
    int nameAndTypeIndex = i & 0xff;
    int d = htonl(*(int *)data[nameAndTypeIndex]);
    i = d >> 16;
    return (data[i]);
};
/**
 * 根据JVM指令中的操作数 获取常量池中方法的描述（参数与返回值的描述）
 * @param index
 * @return
 */
string ConstantPool::getDescriptorNameByMethodInfo(unsigned short index){
    int i = htonl(*(int *)data[index]);
    int nameAndTypeIndex = i & 0xff;
    int d = htonl(*(int *)data[nameAndTypeIndex]);
    i = d & 0xff;
    return (data[i]);
};
/**
 * 根据JVM指令中的操作数 获取常量池中类的全限定名
 * @param index
 * @return
 */
string ConstantPool::getClassNameByMethodInfo(unsigned short index) {
    return getClassNameByFieldInfo(index);
}