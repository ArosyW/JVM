cmake_minimum_required(VERSION 3.16)
project(JVM)

set(CMAKE_CXX_STANDARD 11)

add_executable(JVM main.cpp Stream/ClassRead.cpp Stream/ClassRead.h oop/InstanceKlass.cpp oop/InstanceKlass.h classFile/ClassFileParser.cpp classFile/ClassFileParser.h oop/ConstantPool.cpp oop/ConstantPool.h util/BasicType.h oop/InterfacesInfo.cpp oop/InterfacesInfo.h oop/FieldsInfo.cpp oop/FieldsInfo.h oop/CodeAttributeInfo.cpp oop/CodeAttributeInfo.h inteoreter/BytecodeStream.cpp inteoreter/BytecodeStream.h oop/MethodInfo.cpp oop/MethodInfo.h oop/LineNumberTable.cpp oop/LineNumberTable.h oop/LocalVariableTable.cpp oop/LocalVariableTable.h oop/AttributeInfo.cpp oop/AttributeInfo.h)