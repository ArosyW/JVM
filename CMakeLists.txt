cmake_minimum_required(VERSION 3.16)
project(JVM)

set(CMAKE_CXX_STANDARD 11)

add_executable(JVM main.cpp Stream/ClassRead.cpp Stream/ClassRead.h oop/InstanceKlass.cpp oop/InstanceKlass.h classFile/ClassFileParser.cpp classFile/ClassFileParser.h oop/ConstantPool.cpp oop/ConstantPool.h util/BasicType.h)