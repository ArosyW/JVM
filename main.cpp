#include "Stream/ClassRead.h"
#include "classFile/ClassFileParser.h"
#include "oop/InstanceKlass.h"
#include "classFile/BootClassLoader.h"
#include "runtime/JavaThread.h"
#include "native/JavaNativeInterface.h"
#include "inteoreter/CodeRunBase.h"

using namespace std;

void startVM();

int main() {
    startVM();
    string name = "HelloJVM";
    InstanceKlass *klass = BootClassLoader::loadKlass(name);//加载HelloJVM类
    MethodInfo *m = JavaNativeInterface::getMethod(klass, "main", "([Ljava/lang/String;)V");//遍历klass所有的方法，找到main方法
    JavaThread *javaThread = new JavaThread;//模拟线程的创建
    JavaNativeInterface::callStaticMethod(javaThread,m);//执行main方法
    return 0;
}
void startVM(){
    CodeRunBase::initCodeRun();//初始化字节码解释器，把解析JVM指令的方法全部存入map,以便使用
};