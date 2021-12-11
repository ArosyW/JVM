#include "Stream/ClassRead.h"
#include "classFile/ClassFileParser.h"
#include "oop/InstanceKlass.h"
#include "classFile/BootClassLoader.h"
#include "runtime/JavaThread.h"
#include "native/JavaNativeInterface.h"

using namespace std;

int main() {
    string name = "HelloJVM";
    InstanceKlass *klass = BootClassLoader::loadKlass(name);//加载HelloJVM类
    MethodInfo *m = JavaNativeInterface::getMethod(klass, "main", "([Ljava/lang/String;)V");//遍历klass所有的方法，找到main方法
    JavaThread *javaThread = new JavaThread;//模拟线程的创建
    JavaVFrame *javaVFrame = new JavaVFrame;//马上要执行方法了，先创建栈帧
    javaThread->stack.push(javaVFrame);//栈帧push进线程的栈空间
    JavaNativeInterface::callStaticMethod(javaThread,m);//执行main方法
    return 0;
}