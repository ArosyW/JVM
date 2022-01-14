//
// Created by Arosy on 2021/12/11.
//

#ifndef JVM_JAVANATIVEINTERFACE_H
#define JVM_JAVANATIVEINTERFACE_H


#include "../oop/MethodInfo.h"
#include "../runtime/JavaThread.h"

class JavaNativeInterface {
public:
    /**
     * 找到某个Java类中的某个方法
     * @param klass
     * @param name
     * @param descriptor
     * @return
     */
    static MethodInfo *getMethod(InstanceKlass *klass, string name, string descriptor);
    /**
     * 调用static类型的Java方法
     * @param javaThread
     * @param method
     */
    static void callStaticMethod(JavaThread* javaThread,MethodInfo *method);
     /**
     * invokevirtual 用来调用Java方法
     * @param javaThread
     * @param method
     */
    static void callVirtual(JavaThread *javaThread, MethodInfo *method, int paramCount, char **params);

    /**
     * invokevirtual 用来调用Java方法
     * @param javaThread
     * @param method
     */
    static void callSpecial(JavaThread *javaThread, MethodInfo *method, int paramCount, char **params);
};


#endif //JVM_JAVANATIVEINTERFACE_H
