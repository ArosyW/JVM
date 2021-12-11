//
// Created by Arosy on 2021/11/29.
//

#include "BootClassLoader.h"
#include "ClassFileParser.h"
string BootClassLoader::prePath = "/Users/eba/Documents/github/JDK/out/production/JDK/";
map<string, InstanceKlass *> BootClassLoader::allClass;

InstanceKlass *BootClassLoader::loadKlass(string &p) {
    if (allClass[p] != 0 ) {//先判断此类是否加载过
        return allClass[p];
    }
    string path = p.append(".class");
    string tempPath = prePath;
    InstanceKlass *klass = ClassFileParser::Parser(ClassRead::readByPath(tempPath.append(path)));//读取.class到内存，然后解析它
    allClass[p] = klass;//将解析完的klass存起来
    return klass;
}