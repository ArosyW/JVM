# JVM

## 一、介绍

本项目将尝试性的一步一步实现jvm的主线内容，并对每一次代码的提交进行讲解，每一次讲解都尽可能的假设你一无所知。考虑到来此学习的大部分是Java程序员，所以本项目的C++代码将充满着Java的味道。

* 解惑老师：启明南

* 适合人群：面向java虚拟机的初学者、想要自实现jvm的学习者。
  如果想自己动手，那么你需要有一定的C\C++基础（可以理解指针即可）。如果仅仅是学习，那么相信强大的你不需要学过C\C++也能看懂。

* 测试用例： 将 /Users/e/Documents…… 路径修改为你的自己编译好的java代码的路径即可。

* 如果有哪一章节你看不明白，那一定是我的问题，留言，我将尝试重写那一小节。

* 作者水平有限，若有错误，请指出，最终将会被更新到勘误中。

* 参考资料：《Java虚拟机规范》

* 本系列中的"模版解释器"部分受《揭秘Java虚拟机》 作者：封亚飞 的启发、影响是巨大的，特意说明，以示尊敬

* 如果图片加载失败，你可是技术人员！自己想想办法。（去https://www.ipaddress.com 查 raw.githubusercontent.com 的ip，然后绑host就完事）

---

## 二、目录（不定时更新……）

### (一)类加载模块
#### 0.[一切的开始](#一切的开始)
#### 0.5[初识字节码文件](#初识字节码文件)
#### 1.[字节码文件读取到内存](#字节码文件读取到内存)
#### 2.[解析内存中的字节码文件](#解析内存中的字节码文件)
##### 2.1[解析“魔数”](#魔数)
##### 2.2[解析“版本号”](#版本号)
##### 2.3[解析“Class常量池”](#常量池)
##### 2.4[解析“访问权限”](#访问权限)
##### 2.5[解析“类名 && 父类名”](#类名)
##### 2.6[解析"接口"](#接口)
##### 2.7[解析“字段”](#字段)
##### 2.8[解析“方法”](#方法)
##### 2.9[解析“属性”](#属性)
#### 3.[类加载器](#类加载器)
### (二)执行引擎
#### 1.[cpu是如何执行指令的](#cpu是如何执行指令的)
#### 2.[cpu是如何执行方法的](#cpu是如何执行方法的)
#### 3.[初识JVM执行器](#初识执行器)
##### 3.1[JVM指令集](#JVM指令集)
##### 3.2[参数/返回值的传递](#传递)
##### 3.3[执行器概念](#执行器概念)
##### 3.4[方法执行理论示例](#方法执行理论示例)
#### 4.[字节码解释器](#字节码解释器)
##### 4.1[执行环境](#执行环境)
##### 4.2[指令解释](#指令解释)
###### 4.2.1[JVM指令格式 与 nop指令的实现](#nop)
###### 4.2.2[getstatic指令的实现](#getstatic)
###### 4.2.3[iconst0指令的实现](#iconst0)
###### 4.2.4[putstatic指令的实现](#putstatic)
###### 4.2.5[ldc指令的实现](#ldc)
###### 4.2.6[dup指令的实现](#dup)
###### 4.2.7[aload0、aload1指令的实现](#aload0)
###### 4.2.8[new指令的实现](#new)
###### 4.2.9[invokevirtual指令的实现](#invokevirtual)
###### 4.2.10[invokespecial指令的实现](#invokespecial)
###### 4.2.11[本地方法的调用](#本地方法)
###### 4.2.12[return指令的实现](#return)
##### 4.3[终于完成Hello JVM 的打印](#打印)
###### 4.3.1[自定义JDK的准备](#JDK)
###### 4.3.2[成功打印](#成功)
#### 5.[模版解释器](#模版解释器)
##### 5.1[初步认识](#初步认识)
##### 5.2[机器指令入口CallStub](#CallStub)
###### 5.2.1[CallStub简介](#CallStub简介)
###### 5.2.2[CallStub实现](#CallStub实现)
##### 5.3[方法执行环境的准备entry_point](#entry_point)
#### 6.[执行引擎完结](#执行引擎完结)
### (三)内存池
#### 1.Java进程总内存
#### 2.堆区
#### 3.方法区
### (四)GC
#### 1.标记 && 清除 && 整理
#### 2.新生代 && 老年代 
### (五)即时编译
### (六)扩展内容
### (七)勘误
#### 1.[解析常量池时多循环了一次](#多循环)
#### 2.[LDC指令中因内存失效而产生的bug](#LDC指令)
---
      
## 三、猛男讲解
<br/><br/>
PS：每一章节都会进行单元测试，为了简洁，文章部分地方可能只给出方法的声明，但会给出此次提交的commit,里面包含了方法的实现。
<br/><br/>

### (一)类加载模块


**<p id="一切的开始">0.一切的开始：</p>**

---

<br/>
   
     首先用你的IDEA，新建一个javaClass，输出 “Hello JVM”，并将其运行，相信这对于聪明的你易如反掌，就犹如探囊取物。
```java
       
public class HelloJVM {
    public static void main(String[] args) {
        System.out.println("Hello JVM");
    }
}
       
```
随后你会看到有一个 out 文件生成，里面有一个HelloJVM.class 文件。


<img src="https://github.com/ArosyW/picture/blob/master/outClass.png" width = "450" height = "300" />
没错，这就是IDEA帮你编译后的文件，java虚拟机最终执行的就是这个.class文件，我们先来打开看看它里面是啥玩意。<br/>
<br/>
打开方式：<br/>
1.直接拖入subLime Text (某文本编辑器)<br/>
2.命令行(mac)：
```
vim /Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class
:%!xxd
```
当然你也可以以任何你喜欢的其他方法打开它，最终会看到其内容如下：

<img src="https://github.com/ArosyW/picture/blob/master/class16.jpeg" width = "400" height = "500" />


**<p id="初识字节码文件">0.5初识字节码文件：</p>**

---

<br/>
<br/>
既然我们自定义的HelloJVM类最终被编译成了这个16进制文件，并被Java虚拟机读取解析执行，那一定要满足两个最基本的条件：
<br/><br/>
1.这个16进制文件一定包含了HelloJVM类所有内容<br/>
2.这个16进制文件的内容顺序一定有规范，只有这样Java虚拟机才能按照约定的规律正确读取。
<br/><br/>


我们将通过一边讲解一边写代码解析的方式来知晓它的全貌，整个类加载模块要做的，可以用一句话总结：<br/><br/>
**读取硬盘上的.class文件，并把里面的内容用C++的方式（主要指c++的类文件）按照一定规范存储在内存中，以供需要的时候可以立即检索到。**
<br/><br/>
因此即使类加载模块你看的迷迷糊糊也没有关系，它只是整个Java虚拟机的一小部分，这一部分都是规范，没有太多技巧，就是把你写的Java代码用C++存储起来而已，一定不要放弃，假如这是你第一次接触此类内容，你只需要能够跟着我过一遍就很了不起了，你很幸运，在未来的章节中我将时不时地这么鼓励你。
<br/><br/>
围绕着这句总结，现在我们只有一个问题要讨论：**读取与存储的规范是什么？**
<br/><br/>
我将逐步讲解.class中的每一个字节的含义，小伙伴亦可参考：《Java虚拟机规范》
<br/><br/>
不妨先来认识前4个字节，“cafe babe”，这4个字节将先被Java虚拟机读取，它的含义在于指明这是一个.class文件，你可以继续读取了，否则没有读下去的必要，因为Java虚拟机只能解析.class文件。另外它被取了一个听起来更牛*一点的名字叫做“魔数”，英文名“magic"。
<br/><br/>


**<p id="字节码文件读取到内存">1.字节码文件读取到内存：</p>**

---

   **本次commit :** b898c3ba733fb44c06fec6b0928cd9a173f5cd0f 
   * 读到内存
   新建 ClassRead.h && ClassRead.cpp 
```c++
class ClassRead {
    char *data;
    int cur = 0;
public:
    //构造方法
    ClassRead(char *data);

    //根据路径读取.class文件
    static ClassRead* readByPath(string path);

    unsigned char readByOneByte();//往后读取一个字节
    unsigned short  readByTwoByte();//往后读取两个字节
    unsigned int readByFourByte();//往后读取四个字节

};
```
重申：方法的具体实现请移步代码commit:f4f61fcfe7f461d69a58d01b2e71bb6efe4e5387
<br/><br/>
我们来做一个测试
```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    printf("%X\n", classRead->readByFourByte());
    return 0;
}
```
输出：CAFEBABE 

小总结：到这你已经成功的完成了字节码文件解析的第一步：验证这个文件是否是一个字节码文件！你真厉害，奖励自己一局”亚索“吧！


**<p id="解析内存中的字节码文件">2.解析内存中的字节码文件：</p>**

---


* **<p id="魔数">2.1解析“魔数”：</p>**

   **本次commit :** b51a7f6ab598a13b1e56b6f8cf9686944c10084c

在前文我们已经成功的将.class文件读取到了内存，创建了按1、2、4个字节往后读取的方法，并且读取了前4个字节为CAFEBABE。
<br/><br/>
在解析剩下的字节之前，我们来思考一个问题，在解析这些字节的时候我们应该将他们存储在哪，才能在需要的时候快速定位到？
通常的做法，也是Hotspot的做法，用C++新建一个名为Klass的类，来对应一个.class文件，我们直接上代码。
```c++
class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
    
};
```
我们只解析了前四个字节，所以InstanceKlass目前只定义了一个属性，随着解析的继续，InstanceKlass中将包含所有.class文件的内容。
我们来简短的讨论一下代码结构，前面我们定义了ClassRead来读取磁盘上的.class文件到内存中，很自然的我们还需要有一个专门解析内存中的字节的c++类，于是我们新建类ClassFileParser
```c++
class ClassFileParser {
public:
    static InstanceKlass* Parser(ClassRead *classRead);
    static void checkAndPutMagic(ClassRead *classRead, InstanceKlass *klass);
};
```
它目前只有两个方法：<br/>
1.Parser，就是用来解析被读到内存中的.class文件，也就是在上一章节生成的ClassRead对象。然后返回一个InstanceKlass对象，是不是很像一个InstanceKlass工厂。
<br/>
2.checkAndPutMagic，供Parser调用。<br/><br/>
我们来把前四个字节写入InstanceKlass对象中。
```c++
InstanceKlass *ClassFileParser::Parser(ClassRead *classRead) {
    InstanceKlass *klass = new InstanceKlass//创建klass对象用来存储.class文件中的信息;
    checkAndPutMagic(classRead, klass);//校验是否是class文件
    return klass;
}

void ClassFileParser::checkAndPutMagic(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMagic(classRead->readByFourByte());
    if (klass->getMagic() == MAGIC) { //MAGIC : CAFEBABE
        printf("class文件校验正确\n");
        return;
    }
    printf("class文件校验错误，%X\n", klass->getMagic());
}
```
在Parser中我们new了一个InstanceKlass对象，然后调用checkAndPutMagic将前四个字节赋值给InstanceKlass的magic属性。
<br/><br/>
我们来做一个测试<br/><br/>
```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/eba/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    printf("%X\n", klass->getMagic());
    return 0;
}
```
输出：<br/>
class文件校验正确<br/>
CAFEBABE

小总结：本节我们新建了InstanceKlass用来存储.class文件中的字节信息，并正确的将CAFEBABE属性正确地写入了InstanceKlass对象中，顺便确定了解析流程的代码结构，每次新解析一块内容，只需要新建解析方法，然后Parser方法中调用即可，示例：checkAndPutMagic 方法。

---

* **<p id="版本号">2.2解析“版本号”：</p>**

  **本次commit :** 36344db2cf2e8be84980307cf45d7b1378d4a746

在前文中我们已经成功地将"CAFEBABE"存入到了我们新创建的klass对象，下面我们再来读四个字节，"0000 0034"是用来校验JDK版本号的,其中"0000" 表示JDK的次版本号"minorVersion"，"0034"表示主版本号"majorVersion"，次版本号一般为0，主版本号与JDK版本的对应关系如下：
<br/>

|  jdk版本   | major version  |
|  ----  | ----  |
| 1.1  | 45 |
| 1.2  | 46 |
| 1.3  | 47 |
| 1.4  | 48 |
| 5  | 49 |
| 6  | 50 |
| 7  | 51 |
| 8  | 52 |

<br/>
十六进制的"0034"，即为十进制的52，没错，我用的JDK8。于是我们给InstanceKlass新增两个属性如下：

```c++
class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
    short minorVersion; //JDK次版本号
    short majorVersion; //JDK主版本号
}
```
然后在ClassFileParser中新增一个解析版本号的方法，然后在Parser方法中调用它，于是我们的代码变成了这样：

```c++
InstanceKlass *ClassFileParser::Parser(ClassRead *classRead) {
    InstanceKlass *klass = new InstanceKlass;
    checkAndPutMagic(classRead, klass);//校验是否是class文件
    checkAndPutVersion(classRead, klass);//校验版本号
    return klass;
}

void ClassFileParser::checkAndPutVersion(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMinorVersion(classRead->readByTwoByte());//读取两个字节作为次版本号set进InstanceKlass属性
    klass->setMajorVersion(classRead->readByTwoByte());//读取两个字节作为主版本号set进InstanceKlass属性
    printf("次版本号：%d\n", klass->getMinorVersion());
    printf("主版本号：%d\n", klass->getMajorVersion());
    switch (klass->getMajorVersion()) {//校验版本号是否在允许范围内
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52: {
            if (klass->getMinorVersion() == 0) {
                printf("版本校验正确\n");
                return;
            }
        }
    }
    printf("版本校验错误\n");
};
```
我们来做一个测试：

```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    return 0;
}
```

输出：<br/><br/>
class文件校验正确<br/>
次版本号：0<br/>
主版本号：52<br/>
版本校验正确<br/>
<br/>
小总结：本章节我们新建了属性与方法用于解析"0000 0034"版本号，事实上，接下来解析剩余字节的流程与此类似。

---

* **<p id="常量池">2.3解析“Class常量池”：</p>**

  **本次commit :** 9a212900d255cda698780d96dafff43dc1152677

“常量池”顾名思义，一堆常量的集合，诸如"main"、"HelloJVM.java"等单词都存储在这里面，或许在这之前你有听说过"方法区"以及它的实现"元空间"、"永久代"，没错，从狭义上，InstanceKlass里面存的所有的内容在"方法区"，而"方法区"就是一块内存空间，我们在GC章节会讲解。
若是你亦听说过"方法区"里存储着"元数据"，那么这一章节你将更加清晰的认识到"元数据"是什么，当然，元数据包含整个InstanceKlass里面的内容，而不局限于本章节的Class常量池。
<br/><br/>
PS：后面我们还会讲到"运行时常量池" 和 "字符串常量池"
<br/><br/>
动手！
<br/><br/>
我们会遇到一个问题，Java类是自已定义的，里面究竟有多少个方法，多少个属性是不一定的，意味着"常量池"的大小是不一定的，那么我们如何确定我们应该读取多少个字节停下呢。
显然我们需要预知长度，这里用了两个字节表示长度。"CAFEBABE 0000 0034 0022 ……" 其中0x0022是十进制的34，这里要特别注意，按照约定，真正有效的常量池数量是所给出的长度n-1，意味着我们只有33个常量池字段需要读取，那么用一个for循环33次是比较合理的。
每一次循环即每一次读取常量池字段的时候，我们还需要知道我们读到的数据是什么类型才能知道接下来读取几个字节能完全取到我们想要的数据，这里给出14中类型如下：

|  类型   | 映射值  |
|  ----  | ----  |
|   CONSTANT_Class               |    7 |
|   CONSTANT_Fieldref            |    9 |
|  CONSTANT_Methodref            |   10 |
|  CONSTANT_InterfaceMethodref   |   11 |
|   CONSTANT_String              |    8 |
|   CONSTANT_Integer             |    3 |
|   CONSTANT_Float               |    4 |
|   CONSTANT_Long                |    5 |
|   CONSTANT_Double              |    6 |
|  CONSTANT_NameAndType          |   12 |
|   CONSTANT_Utf8                |    1 |
|  CONSTANT_MethodHandle         |   15 |
|  CONSTANT_MethodType           |   16 |
|  CONSTANT_InvokeDynamic        |   18 |

<br/><br/>
那么解析常量池的流程基本明了了，先读取两个字节获取到常量池的长度count，然后循环count次，每次先读取一个字节用来匹配类型，这里显然用一个switch case比较合理，匹配到了类型，做不同的处理，然后存在InstanceKlass中。

我们先来在创建常量池类ConstantPool:

```c++
class ConstantPool {
public:
    char *tag;
    std::map<int, char*> data;
};

```
用tag来表示数据类型，用map来存储数据，key为索引，也就是它是第几个常量（后面的章节执行指令时需要用到索引），value即为真正的常量池数据。
<br/><br/>
接下来在InstanceKlass中增加常量池数量和常量池两个字段：
```c++
class InstanceKlass {
    int magic; 
    short minorVersion; 
    short majorVersion;
    short constantPoolCount;//常量数量（新增）
    ConstantPool *constantPool;//常量池数据（新增）
};

```
ClassFileParser中新增两个方法分别解析"常量池数量"和"常量池"：

```c++
InstanceKlass *ClassFileParser::Parser(ClassRead *classRead) {
    InstanceKlass *klass = new InstanceKlass;
    checkAndPutMagic(classRead, klass);//校验是否是class文件
    checkAndPutVersion(classRead, klass);//校验版本号
    parserConstantPoolCount(classRead, klass);//解析出常量池数量
    parserConstantPool(classRead, klass);//解析常量池
    return klass;
}

void ClassFileParser::parserConstantPoolCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setConstantPoolCount(classRead->readByTwoByte());
};
void ClassFileParser::parserConstantPool(ClassRead *classRead, InstanceKlass *klass) {
    klass->setConstantPool(new ConstantPool);
    ConstantPool *constantPool = klass->getConstantPool();
    constantPool->tag = new char[klass->getConstantPoolCount()];
    for (int i = 1; i < klass->getConstantPoolCount(); i++) {
        unsigned char tag = classRead->readByOneByte();
        *(constantPool->tag + i) = tag;
        switch (tag) {
        }
    }
};

```
   * 处理方式：
     * utf-8:长度是可变的，规定用2个字节指明了长度，于是先读取两个字节获取长度length，随后读取length个字节的内容，将其存储在ConstantPool中的data中。
     * Integer、Float:读取4个字节将其存储在ConstantPool中的data中。
     * Long、Double:占用两个ConstantPool->data索引，暂不处理。
     * Class、String:读取2个字节将其存储在ConstantPool中的data中。
     * Fieldref、Methodref、InterfaceMethodref、NameAndType: 申请4个字节的内存i，先读取2个字节存储在i的左16位，再读取两个字节存储在i的右16位。
<br/><br/>
     
于是我们的parserConstantPool方法变成了这样：

```c++
void ClassFileParser::parserConstantPool(ClassRead *classRead, InstanceKlass *klass) {
    klass->setConstantPool(new ConstantPool);//为常量池初始化内存空间
    ConstantPool *constantPool = klass->getConstantPool();
    constantPool->tag = new char[klass->getConstantPoolCount()];//根据常量池数量初始化常量池数据区的内存空间

    for (int i = 1; i < klass->getConstantPoolCount(); i++) {
        unsigned char tag = classRead->readByOneByte();//读取一个字节获取到类型的映射值
        *(constantPool->tag + i) = tag;//存储类型
        switch (tag) {//根据不同的类型，有不同的处理方式，主要是读取的字节数不同
            case CONSTANT_Utf8: {
                unsigned short len = classRead->readByTwoByte();//读两个字节作为utf-8的字节长度
                char *target = new char[len + 1]; //申请len+1长度的字节内存，c语言最后一位需要用'\0'填充
                classRead->readByFreeByte(len, target);//读取len个字节
                (constantPool->data[i]) = target;//存储在常量池
                printf("第%d个，类型utf-8，值%s\n", i, constantPool->data[i]);
                break;
            }
            case CONSTANT_Integer: {
                char *temp = new char;
                *temp = classRead->readByFourByte(); //读取四个字节
                constantPool->data[i] = temp;//存储在常量池
                printf("第%d个，类型Integer，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_Float: {
                char *temp = new char;
                *temp = classRead->readByFourByte();//读取四个字节
                constantPool->data[i] = temp;//存储在常量池
                printf("第%d个，类型Float，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_Long: {
                printf("暂不处理\n");
                break;
            }
            case CONSTANT_Double: {
                printf("暂不处理\n");
                break;
            }
            case CONSTANT_Class: {
                char *temp = new char;
                *temp = classRead->readByTwoByte();//读取两个字节
                constantPool->data[i] = temp;//存储在常量池
                printf("第%d个，类型Class，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_String: {
                char *temp = new char[3];
                *temp = classRead->readByTwoByte();//读取两个字节
                temp[2] = '\0';
                constantPool->data[i] = temp;//存储在常量池
                printf("第%d个，类型String，值%d\n", i, *constantPool->data[i]);
                break;
            }
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref: {
                int *temp = new int;//申请四个字节的内存空间
                short classIndex = classRead->readByTwoByte();//读取两个字节
                short nameAndTypeIndex = classRead->readByTwoByte();//读取两个字节
                *temp = htonl(classIndex << 16 | nameAndTypeIndex);//左16位存储classIndex 右16为存储nameAndTypeIndex
                (constantPool->data[i]) = (char *) temp;//存储在常量池
                printf("第%d个，类型file、method、Interface Methodref，值%X\n", i, htonl(*(int *) constantPool->data[i]));
                break;
            }
            case CONSTANT_NameAndType: {
                int *temp = new int;
                short nameIndex = classRead->readByTwoByte();//读取两个字节
                short descriptorIndex = classRead->readByTwoByte();//读取两个字节
                *temp = htonl(nameIndex << 16 | descriptorIndex);//左16位存储classIndex 右16为存储nameAndTypeIndex
                (constantPool->data[i]) = (char *) temp;//存储在常量池
                printf("第%d个，类型NameAndType，值%X\n", i, htonl(*(int *) constantPool->data[i]));
                break;
            }
            default:
                break;
        }

    }
};

```
我们来做一个测试：

```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    return 0;
}

```

输出：<br/><br/>

```
class文件校验正确 	
次版本号：0 	
主版本号：52 	
版本校验正确 	
第1个，类型file、method、Interface Methodref，值60014 	
第2个，类型file、method、Interface Methodref，值150016 	
第3个，类型String，值23 	
第4个，类型file、method、Interface Methodref，值180019 	
第5个，类型Class，值26 	
第6个，类型Class，值27 	
第7个，类型utf-8，值<init> 	
第8个，类型utf-8，值()V 	
第9个，类型utf-8，值Code 	
第10个，类型utf-8，值LineNumberTable 	
第11个，类型utf-8，值LocalVariableTable 	
第12个，类型utf-8，值this 	
第13个，类型utf-8，值LHelloJVM; 	
第14个，类型utf-8，值main 	
第15个，类型utf-8，值([Ljava/lang/String;)V 	
第16个，类型utf-8，值args 	
第17个，类型utf-8，值[Ljava/lang/String; 	
第18个，类型utf-8，值SourceFile 	
第19个，类型utf-8，值HelloJVM.java 	
第20个，类型NameAndType，值70008 	
第21个，类型Class，值28 	
第22个，类型NameAndType，值1D001E 	
第23个，类型utf-8，值Hello JVM 	
第24个，类型Class，值31 	
第25个，类型NameAndType，值200021 	
第26个，类型utf-8，值HelloJVM 	
第27个，类型utf-8，值java/lang/Object 	
第28个，类型utf-8，值java/lang/System 	
第29个，类型utf-8，值out 	
第30个，类型utf-8，值Ljava/io/PrintStream; 	
第31个，类型utf-8，值java/io/PrintStream 	
第32个，类型utf-8，值println 	
第33个，类型utf-8，值(Ljava/lang/String;)V 	

```

小总结：本章节相信聪明的你已经深切地感受到了我们这一模块的工作要领：按照约定把Java类拆解用C++存起来。假如你对于代码理解很困难，没关系，看一下小测试的输出，只需要知道我们是在存储类信息就好。

---

* **<p id="访问权限">2.4解析“访问权限”：</p>**

  **本次commit :** a340887eaa2ef20cb609c3dd90dce7af54c8bb67

本章节较为简单，只有两个字节，用来表示Java类的访问权限，下面给出一张权限枚举表：

|  类型   | 映射值  |
|  ----  | ----  |
|   PUBLIC               |    0x0001 |
|   FINAL            |    0x0010 |
|  SUPER            |   0x0020 |
|  INTERFACE   |   0x0200 |
|   ABSTRACT              |    0x0300 |
|   SYNTHETIC             |    0x1000 |
|   ANNOTATION             |    0x2000 |
|   ENUM             |    0x4000 |

于是我们在InstanceKlass中新增accessFlags属性：
```c++
class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
    short minorVersion; //JDK次版本号
    short majorVersion; //JDK主版本号
    short constantPoolCount;//常量数量
    ConstantPool *constantPool;//常量池数据
    short accessFlags;//类的访问权限 （新增）
};
```
在ClassFileParser中新建parserAccessFlags方法用来解析类的访问权限：

```c++
void ClassFileParser::parserAccessFlags(ClassRead *classRead, InstanceKlass *klass) {
    klass->setAccessFlags(classRead->readByTwoByte());//读取两个字节，set进InstanceKlass属性
    printf("访问权限：%d\n", klass->getAccessFlags());
};

```
记得在Parser方法中调用parserAccessFlags。

我们来做一个测试：

```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    return 0;
}

```

输出：<br/><br/>
访问权限：33 （PUBLIC SUPER）

小总结：本章节太简单不想总结了，就是顽皮。另外，本次提交修复了上一章节在解析常量池时留下的bug,多循环了一次。

---


* **<p id="类名">2.5解析“类名 && 父类名”（全限定名）：</p>**

  **本次commit :** 6ef6e3a6c4d41d86404fd26079b1468f88332f72

先来看一张图，我们已经解析到这里了：

<img src="https://github.com/ArosyW/picture/blob/master/serp.jpeg" width = "400" height = "500" />

<br/><br/>
“类名 && 父类名”也很简单，各自占用两个字节。需要注意的是，这两个字节表示的并不是真正的String类型的类名，而是一个Class常量池（前面章节解析的ConstantPool）索引，例如在此处，往下读两个字节是"00 05"，表示类名的常量池索引为5，
而常量池索引为5的值如下，是26，常量池索引为26的值即为我们的类名"HelloJVM"。另外这个名字是带有路径的，也就是全限定名，在项目中是唯一的，比如其父类名为"java/lang/Object"。

```

……
第4个，类型file、method、Interface Methodref，值180019
第5个，类型Class，值26
第6个，类型Class，值27
……
第26个，类型utf-8，值HelloJVM
……
```
老套路，InstanceKlass新增属性"类名"和"父类名"：

```c++

class InstanceKlass {
int magic; //魔数，CAFEBABE:用来校验是否是.class文件
……
short thisClass;//类名
short superClass;//父类名
}

```
ClassFileParser中新增parserThisClass、parserSuperClass方法分别解析类名与父类名：

```c++

void ClassFileParser::parserThisClass(ClassRead *classRead, InstanceKlass *klass) {
    klass->setThisClass(classRead->readByTwoByte());//读取两个字节，set进InstanceKlass属性
    printf("类名：%X\n", klass->getThisClass());
};

void ClassFileParser::parserSuperClass(ClassRead *classRead, InstanceKlass *klass) {
    klass->setSuperClass(classRead->readByTwoByte());//读取两个字节，set进InstanceKlass属性
    printf("父类名：%X\n", klass->getSuperClass());
};

```
保姆级提示：要记得在总的解析流程Parser方法中调用这两个新增的方法。

我们来做一个测试：

```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    int indexClass = *(klass->getConstantPool()->data[klass->getThisClass()]);//获取常量池索引
    int indexSuperClass = *(klass->getConstantPool()->data[klass->getSuperClass()]);//获取常量池索引
    printf("类名：%s\n", klass->getConstantPool()->data[indexClass]);//再次索引获取name
    printf("父类名：%s\n", klass->getConstantPool()->data[indexSuperClass]);//再次索引获取name
    return 0;
}

```
输出：<br/><br/>
类名：HelloJVM<br/>
父类名：java/lang/Object

小总结：把四个字节set进InstanceKlass对象，总结完了。

---

* **<p id="接口">2.6解析"接口"：</p>**


  **本次commit :** 8c4245d1fb835ddf1803b916477cd9c2df8a1641


由于Java可实现多个接口，意味着接口的数量是可变的，所以先用两个字节指明接口数量count，接下来才是接口。那么我们只需要for循环count次读取接口信息就可以完成接口的解析,所谓的解析，就是往后读取两个字节获得常量池索引，存储到InstanceKlass属性中
,与前文中的解析"类名"一样，这个常量池索引的值又是一个常量池索引，意味着需要在常量池索引两次，就可以获取到接口的全限定名。
<br/>
于是创建C++类InterfacesInfo用来描述接口的常量池索引和名字:
```c++
class InterfacesInfo {
    short constantPoolIndex;
    string interfacesName;
};
```
在InstanceKlass中新增接口数量interfacesCount和接口属性interfaces:

```c++

class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
……
    short interfacesCount;//接口数量
    InterfacesInfo *interfaces;//接口
}

```
ClassFileParser中新建解析接口数量和解析接口的方法parserInterfacesCount、parserInterfaces：

```c++

void ClassFileParser::parserInterfacesCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setInterfacesCount(classRead->readByTwoByte());
    printf("接口数量：%d\n", klass->getInterfacesCount());
}

void ClassFileParser::parserInterfaces(ClassRead *classRead, InstanceKlass *klass) {
    if (klass->getInterfacesCount() == 0) {
        return;//未实现接口就不解析了
    }
    klass->setInterfaces(new InterfacesInfo[klass->getInterfacesCount()]);
    InterfacesInfo *interfaces = klass->getInterfaces();
    for (int i = 0; i < klass->getInterfacesCount(); i++) {
        unsigned short constantPoolIndex = classRead->readByTwoByte();//往后读取两个字节作为常量池索引
        int index = *(klass->getConstantPool()->data[constantPoolIndex]);//获取新的常量池索引
        string name = klass->getConstantPool()->data[index];//第二次索引取得全限定名
        printf("第%d个接口，name:%s,索引位置：%d", i + 1, name.c_str(), constantPoolIndex);
        *(interfaces + i) = *(new InterfacesInfo(constantPoolIndex, name));//将接口set进InstanceKlass属性中
    }
};
```
我们来做一个测试：

```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    return 0;
}
```
输出：<br/><br/>
接口数量：0

<br/>是的，我们的HelloJVM没有实现接口。
<br/><br/>

小总结：本章节共读了4个字节，分别表示接口数量、接口名字的常量池索引，然后将他们存储在InstanceKlass属性中。

---


* **<p id="字段">2.7解析"类字段"：</p>**


  **本次commit :** 0ccbeddf7e67d6bc166e26f6271581dcd2890df7

"类字段"指的是成员变量，为了模拟这种情况，我们修改一下我们的Java代码，新定义一个"out"属性，然后编译，如下：
```java
public class HelloJVM {
    static int out = 9;
    public static void main(String[] args) {
        System.out.println("Hello JVM");
    }
}
```
由于字段的数量也是可变的，所以先由两个字节指明字段数量count，然后每一个字段给出8个字节，按顺序这8个字节表示的含义分别为：
访问权限、name的常量池索引、类型描述的常量池索引、附加属性数量length，最后解析length个附加属性。对这些含义做个解释，
<br/><br/>
访问权限给出权限映射表：

|  类型   | 映射值  | 
|  ----  | ----  |
|   PUBLIC               |    0x0001 |
|   FINAL            |    0x0010 |
|  PRIVATE            |   0x002 |
|  PROTECTED   |   0x0004 |
|   STATIC              |    0x0008 |
|   VOLATILE             |    0x0040 |
|   TRANSIENT             |    0x0080 |
|   SYNTHETIC             |    0x1000 |
|   ENUM             |    0x4000 |

name的常量池索引：用该索引查到常量池ConstantPool可以获得字段的名字。
<br/><br/>
类型描述的常量池索引：用该索引查到常量池ConstantPool可以获得字段的类型描述符号，下面给出类型与描述符的对应关系：

|  常量池类型描述符   | 类型  |
|  ----  | ----  |
|   B               |    byte |
|   C            |    char |
|  D            |   double |
|  F   |   float |
|   I              |    int |
|   J             |    long |
|   S             |    short |
|   Z             |    boolean |
|   V             |    void |
|   L             |    对象类型，如Ljava/lang/Object |

附加属性：字段可以在附加属性有更多的描述，此处我们为0，属性的详细内容放到后面讲。


我们新建FieldsInfo类用来描述字段：
```c++
class FieldsInfo {
    short accessFlag;
    short nameIndex;
    short descriptorIndex;
    short attributesCount;
};
```
InstanceKlass中新增字段数量fieldsCount和字段fieldsInfo：

```c++

class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
……
    short fieldsCount;//字段数量
    FieldsInfo *fieldsInfo;//字段
}

```
ClassFileParser中新建解析接口数量和解析接口的方法parserInterfacesCount、parserInterfaces：
```c++
void ClassFileParser::parserFieldsCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setFieldsCount(classRead->readByTwoByte());//往后读取两个字节set进InstanceKlass中的字段数量属性
    printf("字段数量：%d\n", klass->getFieldsCount());
}

void ClassFileParser::parserFieldsInfo(ClassRead *classRead, InstanceKlass *klass) {
    klass->setFieldsInfo(new FieldsInfo[klass->getFieldsCount()]);//按照字段数量初始化字段内存空间
    FieldsInfo *fieldsInfo = klass->getFieldsInfo();
    for (int i = 0; i < klass->getFieldsCount(); i++) {//循环字段数量count次，解析字段
        FieldsInfo *fields = (fieldsInfo + i);
        fields->setAccessFlag(classRead->readByTwoByte());//往后读取两个字节set进FieldsInfo的访问权限
        fields->setNameIndex(classRead->readByTwoByte());//往后读取两个字节set进FieldsInfo的name
        fields->setDescriptorIndex(classRead->readByTwoByte());//往后读取两个字节set进FieldsInfo的类型描述
        fields->setAttributesCount(classRead->readByTwoByte());//往后读取两个字节set进FieldsInfo的附加属性数量
        printf("field 解析：\n access:%X,\n nameIndex:%X\n descriptorIndex:%X \n attributesCount:%X\n",
               fields->getAccessFlag(), fields->getNameIndex(), fields->getDescriptorIndex(),
               fields->getAttributesCount());
    }
};
```
我们来做一个测试：

```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/qsc/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    printf("字段访问权限：%d\n", klass->getFieldsInfo()->getAccessFlag());
    printf("字段名字：%s\n",klass->getConstantPool()->data[klass->getFieldsInfo()->getNameIndex()]);
    printf("字段类型：%s\n",klass->getConstantPool()->data[klass->getFieldsInfo()->getDescriptorIndex()]);
    printf("字段附加属性数量：%d\n", klass->getFieldsInfo()->getAttributesCount());
    return 0;
}
```
输出：<br/><br/>
字段访问权限：8<br/>
字段名字：out<br/>
字段类型：I<br/>
字段附加属性数量：0<br/>

小总结：按照约定的字段结构成功存储进了InstanceKlass

|  字段数量   | 访问权限  | 名字索引  | 类型索引  |  附加属性数量  | 附加属性  |
|  ----  | ----  | ----  | ----  | ----  | ----  |
|   2字节 |    2字节 |  2字节 |  2字节 | 2字节 | *字节 |

---

* **<p id="方法">2.7解析"方法"：</p>**

  **本次commit :** 0a96cbf9fe2199bdb7cd19d55b29a7d4edcd39e7

方法的数量是不确定的，因此先用两个字节指明方法数量count。下面是每一个方法所包含的属性：

  | 访问权限  | 名字索引  | 方法描述索引  |  附加属性数量  | 附加属性  |
 | ----  | ----  | ----  | ----  | ----  |
 |    2字节 |  2字节 |  2字节 | 2字节 | *字节 |

其中附加属性是用来修饰这个方法的一些额外的信息，例如方法一般包括一个称为Code的属性，是JVM提供的指令的集合，用来描述这个方法如何执行的，"指令"后面在执行器篇将会详细讲解，此处我们仅仅需要把它存储在InstanceKlass即可。

其他的附加属性过多，不列在这里了，可以查阅《Java虚拟机规范》4.7章节。

下面给出附加属性Code的结构：

 | 名字索引  | 当前属性长度  |  最大栈深度  | 当前方法的局部变量表个数  | Code（指令）长度length  | 指令  | exception_table_length  |  exception_table |  附加属性数量  | 附加属性  |
 | ----  | ----  | ----  | ----  | ----  | ----  |  ----  |  ----  | ----  |  ----  | 
|    2字节 |  4字节 |  2字节 | 2字节 | 4字节  | length字节 | 2字节  | *字节 | 2字节  | *字节


想必看完这么长的表格你已经疯掉了吧，稍微解释一下。

* 名字索引：Class常量池索引
* 当前属性长度：当前属性即名为"Code"的属性，指明了这个属性此次占用多少字节。
* 最大栈深度：方法在执行的时候会在栈中创建"栈帧"，也称为"操作数栈"，最大栈深度即指方法在执行时操作数栈中最大容纳的操作数的数量。
* 当前方法的局部变量表个数：方法中声明的局部变量的数量
* Code（指令）长度length：当前方法的JVM指令的个数
* 指令：当前方法的JVM指令
* exception_table_length：异常处理器的个数，一般方法中有try、catch才会有exception_table。
* exception_table：包含4个属性，我们暂不处理异常，可查阅《Java虚拟机规范》4.7.3
* 附加属性：一般为LineNumberTable、LocalVariableTable，属于调试信息，不是运行时必须的，不必太关注，按照约定存储起来就好。
  * LineNumberTable：

    | name的常量池索引  | 属性长度  | tableLength  |  table 
    | ----  | ----  | ----  | ----  | 
    |    2字节 |  4字节 |  2字节 |  *字节 | 

      给出table的结构：

      | start_pc  | 行号  | 
    | ----  | ----  | 
    |    2字节 |  2字节 |  

  * LocalVariableTable：
  
    | name的常量池索引  | 属性长度  | 局部变量Length   |  table
    | ----  | ----  | ----  | ----  
    |    2字节 |  4字节 |  2字节 |  *字节

    给出局部变量table的结构：

    | start_pc  | length  | name常量池索引  | 类型的常量池索引  | 局部变量表索引
    | ----  | ----  | ----  | ----  | ----  | 
    |    2字节 |  2字节 |   2字节 |   2字节 |  2字节 |  

<br/>
方法的解析东西略微多一些，显然我们需要嵌套for循环去解析，上面已经列出了我们解析所需要的所有约定，大部分与我们初步实现JVM无关，按约定解析就好，无需纠结他们是做什么用的。需要重点关注的是"指令"，这是我们后面执行方法所依赖的。
<br/>

于是我们需要新建C++类来映射这些属性，以达到存储的目的：
MethodInfo、CodeAttributeInfo、AttributeInfo、LineNumberTable、LocalVariableTable、BytecodeStream。

新增方法parserMethodCount、parserMethod、parserLineNumberTable、parseLocalVariableTable分别用来解析方法数量、解析方法、解析LineNumberTable、解析LocalVariableTable。

重点看一下parserMethod方法：
```c++
void ClassFileParser::parserMethodInfo(ClassRead *classRead, InstanceKlass *klass) {
    klass->setMethodInfo(new MethodInfo[klass->getMethodCount()]);//初始化InstanceKlass中的methodInfo的内存空间
    for (int i = 0; i < klass->getMethodCount(); i++) {
        MethodInfo *method = new MethodInfo;
        method->setBelongKlass(klass);//将方法与所属InstanceKlass关联
        method->setAccessFlags(classRead->readByTwoByte());//存储方法的访问权限
        method->setNameIndex(classRead->readByTwoByte());//存储方法的访问权限
        method->setMethodName((string) klass->getConstantPool()->data[method->getNameIndex()]);//存储方法的名字
        printf("解析方法%s\n", method->getMethodName().c_str());
        method->setDescriptorIndex(classRead->readByTwoByte());//存储方法的描述（包含了参数、返回值）
        method->setAttributesCount(classRead->readByTwoByte());//存储属性数量
        method->initCodeAttributeInfo();
        *(klass->getMethodInfo() + i) = *method;
        for (int j = 0; j < method->getAttributesCount(); j++) {
            CodeAttributeInfo *codeAttributeInfo = new CodeAttributeInfo;
            codeAttributeInfo->setAttrNameIndex(classRead->readByTwoByte());//存储属性名字的常量池索引
            codeAttributeInfo->setAttrLength(classRead->readByFourByte());//存储属性长度
            codeAttributeInfo->setMaxStack(classRead->readByTwoByte());//存储最大栈深度
            codeAttributeInfo->setMaxLocals(classRead->readByTwoByte());//存储局部变量表数量
            codeAttributeInfo->setCodeLength(classRead->readByFourByte());//存储指令数量
            BytecodeStream *bytecodeStream = new BytecodeStream(method, codeAttributeInfo,
                                                                codeAttributeInfo->getCodeLength(), 0,
                                                                new char[codeAttributeInfo->getCodeLength()]);
            classRead->readByFreeByte(codeAttributeInfo->getCodeLength(), bytecodeStream->getCodes()); //重点： 将方法的JVM指令存储在bytecodeStream
            codeAttributeInfo->setCode(bytecodeStream);
            printf("\t第%d个属性，access flag:%X name index : %X  stack:%X container:%X  code length:%X \n", j,
                   method->getAccessFlags(), codeAttributeInfo->getAttrNameIndex(), codeAttributeInfo->getMaxStack(),
                   codeAttributeInfo->getMaxLocals(), codeAttributeInfo->getCodeLength());
            codeAttributeInfo->setExceptionTableLength(classRead->readByTwoByte());//存储Exception表长度，此处暂时为0，因为我们没有任何异常需要处理
            codeAttributeInfo->setAttributesCount(classRead->readByTwoByte());//存储属性长度
            method->setAttributeInfo(codeAttributeInfo, j);
            for (int k = 0; k < codeAttributeInfo->getAttributesCount(); k++) {//循环解析属性
                int nameIndex = classRead->readByTwoByte();
                string attrName = klass->getConstantPool()->data[nameIndex];
                if ("LineNumberTable" == attrName) {//解析LineNumberTable
                    parserLineNumberTable(classRead, codeAttributeInfo, attrName, nameIndex, klass);
                } else if ("LocalVariableTable" == attrName) {//解析LocalVariableTable
                    parseLocalVariableTable(classRead, codeAttributeInfo, attrName, nameIndex, klass);
                }
            }
        }
    }
};

```

更详细的解析请看本次commit: 0a96cbf9fe2199bdb7cd19d55b29a7d4edcd39e7
<br/><br/>
我们来做一个测试：

```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/qsc/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    return 0;
}
```
<br/>

输出：<br/>

```
方法数量：3
解析方法<init>
第0个属性，access flag:1 name index : B  stack:1 container:1  code length:5
lineNumberTable: name index:12,attr len:6, table len:1
第0个属性，start pc : 0,line numnber:1
LocalVariableTable:第0个属性，start pc:0,length:5,name index:14,descrip:15,index:0
解析方法main
第0个属性，access flag:9 name index : B  stack:2 container:1  code length:8
lineNumberTable: name index:12,attr len:10, table len:2
第0个属性，start pc : 0,line numnber:4
lineNumberTable: name index:12,attr len:10, table len:2
第1个属性，start pc : 7,line numnber:5
LocalVariableTable:第0个属性，start pc:0,length:8,name index:18,descrip:19,index:0
解析方法<clinit>
第0个属性，access flag:8 name index : B  stack:1 container:0  code length:6
lineNumberTable: name index:12,attr len:6, table len:1
第0个属性，start pc : 0,line numnber:2
```
小总结：如果这对你来说过于陌生，你只需要知道我们把方法的JVM指令存起来了就可以跳过本部分了。另外.class的解析我们已经走了99%了。

---

* **<p id="属性">2.7解析"属性"：</p>**

  **本次commit :** 4a6fbf0dda391153ea2eb8f706cff67bfb7655ba

<br/>
有个好消息，解析"属性"是.class文件解析的最后一章节。

此处的属性与我们在解析字段和方法时遇到的"附加属性"是相同的结构，由于属性的枚举数量过多，此处不给出了，详细的枚举解析可查阅《Java虚拟机规范》4.7章节。
我们的Java类"HelloJVM"是包含一个称为"SourceFile"的属性。同LineNumberTable、LocalVariableTable一样，属于调试信息，不是运行时必须的，不必过于关注。
按照下面给出的约定存储在InstanceKlass中就好。

|  属性name的常量池索引  | 属性长度length   | 文件名的常量池索引  | 
|  ----   |  ----   | ----   | 
|  2字节  |  4字节  |   2字节  | 

于是我们在InstanceKlass中新增attributeCount属性数量、attributeInfo属性：
```c++

class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
……
    short attributeCount;
    AttributeInfo *attributeInfo;
}

```
在ClassFileParser中新增parserAttributeCount、parserAttribute方法用来解析属性数量和属性：

```c++

void ClassFileParser::parserAttributeCount(ClassRead *classRead, InstanceKlass *klass) {
    klass->setAttributeCount(classRead->readByTwoByte());//读取两个字节将属性数量存储在InstanceKlass中
    printf("解析属性，数量%d\n", klass->getAttributeCount());
};

void ClassFileParser::parserAttribute(ClassRead *classRead, InstanceKlass *klass) {
    for (int i = 0; i < klass->getAttributeCount(); i++) {
        short nameIndex = classRead->readByTwoByte();
        string attrName = klass->getConstantPool()->data[nameIndex];
        if ("SourceFile" == attrName) {
        printf("\tSourceFile\n");
        parseSourceFile(classRead, klass, nameIndex, i);//解析SourceFile属性
        } else {
            printf("\t无法识别的属性:%X\n", attrName.c_str());
        }
    }
};

void ClassFileParser::parseSourceFile(ClassRead *classRead, InstanceKlass *klass, short nameIndex, short index) {
    klass->setAttributeInfo(new AttributeInfo[klass->getAttributeCount()]);//初始化内存空间来存储属性
    AttributeInfo *attributeInfo= klass->getAttributeInfo();
    *(klass->getAttributeInfo() + index) = *attributeInfo;
    attributeInfo->setAttributeNameIndex(nameIndex);//存储属性name的常量池索引
    attributeInfo->setAttributeLength(classRead->readByFourByte());//存储属性长度length
    attributeInfo->initContainer();
    *(attributeInfo->getContainer()) = classRead->readByTwoByte();//存储文件名的常量池索引
    printf("\t\t第%d个属性，%s:nameIndex:%d,length:%d,data:%d,(%s)\n", index, klass->getConstantPool()->data[nameIndex],
    attributeInfo->getAttributeNameIndex(), attributeInfo->getAttributeLength(), *attributeInfo->getContainer(),
    klass->getConstantPool()->data[*attributeInfo->getContainer()]);
};


```
<br/><br/>
我们来做一个测试：

```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    InstanceKlass *klass = ClassFileParser::Parser(classRead);
    int attrIndex = klass->getAttributeInfo()->getAttributeNameIndex();
    int length = klass->getAttributeInfo()->getAttributeLength();
    short fileIndex = *(short *)(klass->getAttributeInfo()->getContainer());
    printf("解析属性完成，属性名字：%s,长度：%d，文件名字：%s\n", klass->getConstantPool()->data[attrIndex],
           length,
           klass->getConstantPool()->data[fileIndex]);
    return 0;
}
```
<br/>

输出：<br/>

解析属性完成，属性名字：SourceFile,长度：2，文件名字：HelloJVM.java

小总结：由于我们的Java类HelloJVM只有一个SourceFile属性，为了快速的实现我们的简陋的JVM，我们此处仅仅写了这一种属性的解析逻辑。
再次指路，其他属性的结构请移步《Java虚拟机规范》4.7章节。


**<p id="类加载器">3.类加载器：</p>**

---
* 新增类加载器
  
  **本次commit :** e6906988a7e5e1b89c6197d2acefb7c623a726d8

<br/>
在前文中我们实现了.class文件的解析，
但一个类的加载不仅仅有.class文件的解析，后续我们来会有其他操作，例如执行’clinit‘
方法，因此我们继续对ClassFileParser进行封装，直到一个Java类能够真正的被初始化完成，然后才能对外使用。
于是我们新建BootClassLoader:

```c++
class BootClassLoader {
public:
    static string prePath;//路径前缀 ：项目地址
    static map<string, InstanceKlass *> allClass;//所有加载完成可以使用的Java类
    static InstanceKlass* loadKlass(string& path);//根据路径（全限定名）加载Java类
};
```
 
用loadKlass方法加载Java类，用allClass存储所有被加载完成的Java类：
 
```c++
InstanceKlass *BootClassLoader::loadKlass(string &p) {
    string path = p.append(".class");
    string tempPath = prePath;
    InstanceKlass *klass = ClassFileParser::Parser(ClassRead::readByPath(tempPath.append(path)));
    allClass[p] = klass;
    return klass;
}
```

为了避免重复加载，我们在方法的开始加一个逻辑，先通过allClass判断这个类是否已经加载过了：
 
```c++
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
```
 
即使这样，我们的loadKlass仍然是不完善的，在后面的章节我们会对它进行加锁以及介绍并执行‘clinit‘方法。

<br/>
我们来做一个测试：

```c++
int main() {
    string path = "HelloJVM";
    InstanceKlass *klass = BootClassLoader::loadKlass(path);
    int index = *klass->getConstantPool()->data[klass->getThisClass()];//获取该类的类名索引
    printf("%s\n", klass->getConstantPool()->data[index]);//取出类名
    return 0;
}
```
<br/>

输出：<br/>

HelloJVM

小总结：为了后面完整的加载Java类，我们新增了类加载器，并用map将加载完成的类存起来，当然为了更快的实现这个简陋的JVM，我们不再考虑双亲委派等加载实现。当然这里缺少线程安全的逻辑，下次一定。
<br/><br/>
 
### (二)执行引擎

---

Java方法的执行就是字节码执行器执行指令的过程。有必要先讲一下cpu是如何执行指令的。

<br/><br/>
 **<p id="cpu是如何执行指令的">1.cpu是如何执行指令的：</p>**
 
 ---

  计算机所能执行的只有机器码，例如x86指令集中的：
  <br/>  <br/>
    FNOP       无操作        机器码 ： DA E9
  <br/>  <br/>
 cpu会从一段连续的内存空间的起始位置开始一个字节一个字节的读取，通过译码器去匹配"指令集"，如果匹配不到就继续取一个字节，直到匹配到正确的指令（匹配这件事情是译码器的工作，我们无需过于关注）。每一条指令都有特定的"操作数"，其实就是这条指令的"参数"，下面给出一个例子：
<br/>  <br/>
  MOV AL,1
  <br/>  <br/>
这条指令的意思是将AL寄存器的值设置为 1 ，它的机器码是1011 0000 0000 0001 （二进制），当cpu读取到前八位"操作码"：1011 0000时就可以通过译码器知道操作的是：为AL寄存器赋值。赋值为多少呢？后面紧
跟一个字节的"操作数"就是要赋的值，那么如何知道后面跟的参数的长度呢？其实已经包含在了前八位的操作码里，当cpu读到这个操作码时就默认后面跟一个8位的操作数。
那么cpu只需要再往后读8位得到0000 0001就可以完整地完成这条指令的执行。可以说"操作码"与"操作数"的长度是约定好的。
  <br/>  <br/>
 
 **<p id="cpu是如何执行方法的">2.cpu是如何执行方法的：</p>**
 
 ---
>温柔的提示：假如这一章节你不感兴趣或者看不懂，没关系，这节课可以逃掉，直接看**总结**，不影响手写后面的**字节码解释器**，也不影响你开劳斯莱斯，但当我们手写到**模版解释器**的时候，你不得不将这一章节彻底整明白。


**而所谓的方法，就是多个指令的集合，且在一段连续的内存空间内**，在上文中，cpu执行完一条指令时，会继续往下读取下一条指令，当译码器无法匹配到合法的指令时将会发生异常。既然所有的方法都会被编译成机器码在一段连续的内存空间中存放着，那么想要执行这个方法，只需要让cpu跳转到这段内存空间的首地址即可，那就需要一个能实现跳转的指令：jmp，它是无条件跳转指令。
<br/><br/>
但它自己并不能完成方法的调用，因为它无法"返回"调用者继续执行调用者的代码。因此我们需要的是:call指令配合ret指令，它可以保存调用者的下一条指令，当执行ret完时自动执行调用者的下一条指令。
<br/><br/>
还有一个问题没有解决，就是参数与返回值如何传递。来一段代码：

```c++

int func(int arg, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7) {
    return arg + arg1 + arg2 + arg3 + arg4 + arg5 + arg6 + arg7;
}
int main() {
    func(1, 2, 3, 4, 5, 6, 7, 8);
    return 0;
}

```

编译后：

```c++
main: 
 pushq  %rbp //保存调用者栈底地址（8个字节），即将它入栈
 movq   %rsp, %rbp // 将rsp寄存器存储的值 赋值 给 rbp寄存器
 subq   $0x20, %rsp // 将rsp寄存器所指向的内存空间向下移动32个字节，就是分配32字节的内存
 movl   $0x0, -0x4(%rbp) // 将0赋值给rbp寄存器往下4个字节的位置
 movl   $0x1, %edi //将1 赋值给edi寄存器
 movl   $0x2, %esi // 将 2 赋值给esi寄存器
 movl   $0x3, %edx // 将 3 赋值给edx寄存器
 movl   $0x4, %ecx //将 4 赋值给ecx寄存器
 movl   $0x5, %r8d //将 5 赋值给r8d寄存器
 movl   $0x6, %r9d //将 6 赋值给 r9d寄存器
 movl   $0x7, (%rsp) // 将 7 赋值给rsp寄存器所指向的内存
 movl   $0x8, 0x8(%rsp) // 将 8 赋值给rsp寄存器所指向的内存 往上8个字节的位置
 callq  func // 调用 func函数
 xorl   %ecx, %ecx
 movl   %eax, -0x8(%rbp) // 将eax寄存器的值赋值给rbp寄存器往下8个字节的位置
 movl   %ecx, %eax
 addq   $0x20, %rsp //将rps寄存器所存的内存地址往上32个字节
 popq   %rbp // 弹出栈中内容，赋值给rbp寄存器。并将rsp往上移动8个字节
 retq   // 弹出栈中内容，赋值给ip寄存器，cpu跳转执行

func:
 pushq  %rbp //保存调用者栈底地址（8个字节），即将它入栈
 movq   %rsp, %rbp // 将rsp寄存器存储的值 赋值 给 rbp寄存器
 movl   0x18(%rbp), %eax // 将rbp寄存器所指向的内存 往上24个字节的位置的值 赋值给eax寄存器
 movl   0x10(%rbp), %r10d // 将rbp寄存器所指向的内存 往上16个字节的位置的值 赋值给r10d寄存器
 movl   %edi, -0x4(%rbp) // 将 edi寄存器的值赋值给 rbp寄存器所指向的内存 往下4个字节的位置的值
 movl   %esi, -0x8(%rbp) // 将 esi寄存器的值赋值给 rbp寄存器所指向的内存 往下8个字节的位置的值
 movl   %edx, -0xc(%rbp) // 将 edx寄存器的值赋值给 rbp寄存器所指向的内存 往下12个字节的位置的值
 movl   %ecx, -0x10(%rbp) // 将 ecx寄存器的值赋值给 rbp寄存器所指向的内存 往下16个字节的位置的值
 movl   %r8d, -0x14(%rbp) // 将 r8d寄存器的值赋值给 rbp寄存器所指向的内存 往下20个字节的位置的值
 movl   %r9d, -0x18(%rbp) // 将 r9d寄存器的值赋值给 rbp寄存器所指向的内存 往下24个字节的位置的值
 movl   -0x4(%rbp), %ecx // 将rbp寄存器所指向的内存 往下4个字节的位置的值 赋值给ecx寄存器
 addl   -0x8(%rbp), %ecx // 将rbp寄存器所指向的内存 往下8个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
 addl   -0xc(%rbp), %ecx // 将rbp寄存器所指向的内存 往下12个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
 addl   -0x10(%rbp), %ecx // 将rbp寄存器所指向的内存 往下16个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
 addl   -0x14(%rbp), %ecx // 将rbp寄存器所指向的内存 往下20个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
 addl   -0x18(%rbp), %ecx // 将rbp寄存器所指向的内存 往下24个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
 addl   0x10(%rbp), %ecx // 将rbp寄存器所指向的内存 往上10个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
 addl   0x18(%rbp), %ecx // 将rbp寄存器所指向的内存 往上18个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
 movl   %eax, -0x1c(%rbp) // 将eax寄存器的值 赋值给 rbp寄存器所指向的内存 往下28个字节的位置的值
 movl   %ecx, %eax // 将ecx寄存器中的值 赋值给 eax寄存器
 popq   %rbp // 弹出栈中内容，赋值给rbp寄存器。并将rsp往上移动8个字节
 retq   // 弹出栈中内容，赋值给ip寄存器，cpu跳转执行
```

别怕，去重后一共也没几个指令，学废它！当然cpu最终执行的是0和1的机器码，这里用汇编语言表示，是为了我们理解。
<br/><br/>
从main函数开始，前两条指令：

```

pushq  %rbp 
movq   %rsp, %rbp

```
在func函数开头也是这两条指令，它是为了恢复到调用者而设计的，到func函数我们在讲。

到这你只需要知道：
* rbp寄存器始终指向函数栈的栈底
* rsp寄存器始终指向函数栈的栈顶

>PS： 所谓的栈其实就是一块连续的内存空间，用两个寄存器分别指向它的首地址和尾地址。

那么刚进入main函数执行完上面两条（pushq、movq）通用指令后我们的main函数图就是这个样子：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/maininit.png" width = "660" height = "500" />
<br/><br/>
红色的内容我们进入func方法再讲。
<br/><br/>
下一条指令，说人话就是栈顶（rsp寄存器）下移32个字节，栈的增长是向下的，因此这里就是为main函数分配了32个字节的栈空间：

```

subq   $0x20, %rsp

```

<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainsub.png" width = "600" height = "500" />
<br/><br/>

main函数第四条指令：

```
movl   $0x0, -0x4(%rbp)
```

将 0 赋值给 rbp寄存器指向的内存往下四个字节的位置（这个0其实就是我们main函数的返回值），于是我们的内存图变成了这个样子：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainmov0.png" width = "600" height = "500" />
<br/><br/>

我们一共传递给func函数8个参数， 接下来6条指令就是将前6个参数存入寄存器：

```
movl   $0x1, %edi //将1 赋值给edi寄存器
movl   $0x2, %esi // 将 2 赋值给esi寄存器
movl   $0x3, %edx // 将 3 赋值给edx寄存器
movl   $0x4, %ecx //将 4 赋值给ecx寄存器
movl   $0x5, %r8d //将 5 赋值给r8d寄存器
movl   $0x6, %r9d //将 6 赋值给 r9d寄存器
```
这样当执行进func函数的时候，func函数就可以直接从寄存器中取出前6个参数了，看下现在的内存图：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainmov1-6.png" width = "600" height = "500" />
<br/><br/>

为什么只传递了6个参数？
> 因为寄存器的不够用了，寄存器是十分稀缺的。这也是我为什么要传递8个参数，因为大于6个参数编译器才会用栈内存来传递剩余的参数。

为什么不直接使用栈内存：
> 因为寄存器更快，当然优先用快的方式传递参数。

寄存器为什么快：
> 1.在设计上寄存器离cpu更近。
> 2.寄存器的制作工艺，硬件设计更优越。

为什么内存不采用寄存器的工艺：
>太贵！

**当然，如果你手写汇编，你可以自由选择用、不用、用多少寄存器来传递参数，6个只是编译器编译c语言的限制。**

<br/>
继续我们的main函数，还有两个参数是通过栈内存传递的：
<br/>

```
movl   $0x7, (%rsp) // 将 7 赋值给rsp寄存器所指向的内存
movl   $0x8, 0x8(%rsp) // 将 8 赋值给rsp寄存器所指向的内存 往上8个字节的位置
```

将7、8存入main函数的栈内存中：

<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainmov7-8.png" width = "600" height = "500" />
<br/><br/>

终于我们准备好了func函数需要的8个参数，于是

```
 callq  func // 调用 func函数
```

注意这条指令包含的内容是很丰富的，会先把main函数下一条指令也就是：xorl   %ecx, %ecx ，入栈,然后才会调用func函数。具体操作为把rsp寄存器存储的地址下移8个字节，并把xorl   %ecx, %ecx 存入这8个字节，看图：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfunccall.png" width = "600" height = "500" />
<br/><br/>

下面才进入func函数，一上来就是在main函数开头我们没讲的那两条指令：

```

pushq  %rbp 
movq   %rsp, %rbp

```
因为每个函数的开头都是它，作用一样，所以就放在这里讲了：

>它的用意在于将调用者（func的调用者为main）的栈底地址保存起来（入栈），因为执行当前方法（func）就需要将rbp、rsp指向当前方法（func）的栈底、栈顶。
<br/><br/>
>保存调用者（main）栈底地址，方便当前方法（func）执行完后能让rbp、rsp重新指向调用者（main）的栈底、栈顶。

具体操作为，(pushq  %rbp) 将rbp寄存器中的值（main函数栈底）入栈：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfunpush.png" width = "600" height = "500" />
<br/><br/>

(movq   %rsp, %rbp)将rsp寄存器中的值赋值给rbp寄存器，那么现在rbp、rsp寄存器都指向func函数的栈底了：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfuncrbp.png" width = "600" height = "500" />
<br/><br/>


我们回头看main函数在执行完 " pushq  %rbp ；  movq  %rsp, %rbp " 这两条指令后执行了subq，也就是为当前方法分配栈内存，于是你会**愤怒地发现**（修饰动词用'土''也' DE）func函数竟然没有这一步，导致rsp、rbp寄存器
在整个func函数往后的生命中都一直指向func栈底。
<br/><br/>
因为func函数没有再调用其他任何函数，这种情况下func函数有一个很吊的名字"叶子函数"，部分架构下，这种函数栈底往下128字节，叫作红色区域，默认可以使用，可以选择不分配，128字节不够用的时候还是必须分配的。
<br/><br/>

继续，func接下来八条指令，在做的事情只有一个，把8个参数从寄存器、main栈内存，复制到自己的栈内存中，或者寄存器中，用java的话来讲就是把参数存入到自己的局部变量表。
先看前两条指令：

```
movl   0x18(%rbp), %eax // 将rbp寄存器所指向的内存 往上24个字节的位置的值 赋值给eax寄存器
movl   0x10(%rbp), %r10d // 将rbp寄存器所指向的内存 往上16个字节的位置的值 赋值给r10d寄存器
```
我们回看一下内存图，rbp寄存器所指向的内存，往上24个字节和16个字节的位置，是啥？
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfunc7-8.png" width = "600" height = "500" />
<br/><br/>

天呐，那不就是main函数因为寄存器不够用而把7、8两个参数入栈的位置嘛，惊掉你的下巴了吗，收一收，牙龈露出来了。
<br/><br/>
再来看下面六条指令：

```
 movl   %edi, -0x4(%rbp) // 将 edi寄存器的值赋值给 rbp寄存器所指向的内存 往下4个字节的位置的值
 movl   %esi, -0x8(%rbp) // 将 esi寄存器的值赋值给 rbp寄存器所指向的内存 往下8个字节的位置的值
 movl   %edx, -0xc(%rbp) // 将 edx寄存器的值赋值给 rbp寄存器所指向的内存 往下12个字节的位置的值
 movl   %ecx, -0x10(%rbp) // 将 ecx寄存器的值赋值给 rbp寄存器所指向的内存 往下16个字节的位置的值
 movl   %r8d, -0x14(%rbp) // 将 r8d寄存器的值赋值给 rbp寄存器所指向的内存 往下20个字节的位置的值
 movl   %r9d, -0x18(%rbp) // 将 r9d寄存器的值赋值给 rbp寄存器所指向的内存 往下24个字节的位置的值
```
把这六个寄存器中的值赋值到func栈内存中，而这6个寄存器中的值就是main函数提前写入的1、2、3、4、5、6，于是我们的内存图变成了这个样子：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfunc1-6.png" width = "600" height = "500" />
<br/><br/>

到这func函数已经把它需要的所有参数复制了一份自己用，那6个存储参数的寄存器其实已经完成传递参数的使命了，因为其中的值已经被存到了内存中，那么6个寄存器可以继续用作它用了，
比如接下来进行加法运算的8条指令，就用了ecx寄存器中转结果值：

```
movl   -0x4(%rbp), %ecx // 将rbp寄存器所指向的内存 往下4个字节的位置的值 赋值给ecx寄存器
addl   -0x8(%rbp), %ecx // 将rbp寄存器所指向的内存 往下8个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
addl   -0xc(%rbp), %ecx // 将rbp寄存器所指向的内存 往下12个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
addl   -0x10(%rbp), %ecx // 将rbp寄存器所指向的内存 往下16个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
addl   -0x14(%rbp), %ecx // 将rbp寄存器所指向的内存 往下20个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
addl   -0x18(%rbp), %ecx // 将rbp寄存器所指向的内存 往下24个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
addl   0x10(%rbp), %ecx // 将rbp寄存器所指向的内存 往上10个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
addl   0x18(%rbp), %ecx // 将rbp寄存器所指向的内存 往上18个字节的位置的值 与 ecx寄存器 的值相加 然后赋值回ecx寄存器
```

其中第一条指令，"movl   -0x4(%rbp), %ecx" ，rbp寄存器所指向的内存 往下4个字节的位置的值是我们的参数：1，把它赋值给ecx寄存器：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfuncecx1.png" width = "600" height = "500" />
<br/><br/>

下面讲一条add指令（addl   -0x8(%rbp), %ecx），其余6条同理。

首先-0x8(%rbp)，rbp寄存器指向的内存往下8个字节的位置的值，是 2 ：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfuncaddabout2.png" width = "600" height = "500" />
<br/><br/>

而此时ecx寄存器中的值是 1。

-0x8(%rbp) =2 ，ecx =1 ，那么 （addl   -0x8(%rbp), %ecx） 的意思就是把1 、 2 相加，结果写入ecx寄存器，于是ecx寄存器中的值就是3了：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfuncecx3.png" width = "600" height = "500" />
<br/><br/>

下面6条add指令中的<br/><br/>
 -0xc(%rbp) 、 -0x10(%rbp) 、 -0x14(%rbp) 、 -0x18(%rbp) 、 0x10(%rbp) 、 0x18(%rbp)
<br/><br/>
分别是3、4、5、6、7、8 。 那么就是把ecx寄存器中的值：3，加3、4、5、6、7、8，最后ecx寄存器的值为 36 ：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfuncecx36.png" width = "600" height = "500" />
<br/><br/>

终于，func函数还剩4条指令了，挺住，先来看两条：

```
movl   %eax, -0x1c(%rbp) // 将eax寄存器的值 赋值给 rbp寄存器所指向的内存 往下28个字节的位置的值
movl   %ecx, %eax // 将ecx寄存器中的值 赋值给 eax寄存器
```
还记得eax寄存器中存的啥吗，参数 8 ，这是没有被存入func函数栈内存的，因为后面需要用eax寄存器来存储返回值，8会被覆盖掉，所以需要先存到栈中。
<br/><br/>
而ecx寄存器中存的是我们求和的最终结果，现在将它存入eax寄存器，看执行完这两条指令的图：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfunctoeax.png" width = "600" height = "500" />
<br/><br/>

func函数最后两条指令：
```
popq   %rbp // 弹出栈中内容，赋值给rbp寄存器。并将rsp往上移动8个字节
retq   // 弹出栈中内容，赋值给ip寄存器，cpu跳转执行
```
就是为了将rsp、rbp寄存器恢复到main函数栈顶、栈底。
<br/><br/>
先看（popq   %rbp），将栈顶数据弹出，赋值给rbp寄存器，虽然我们的func函数使用了许多栈空间用来存储参数，但pop指令所认为的栈顶，始终是rsp寄存器所指向的位置，再贴张图看下栈顶位置是什么吧：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfuncpop.png" width = "600" height = "500" />
<br/><br/>
栈顶是main函数的栈底地址，将它赋值给rbp寄存器，这样rbp寄存器就重新指向main函数的栈底了。
<br/><br/>
既然是pop出数据，那么栈顶位置（rsp寄存器）也应该随之改变，即往上移动8个字节，看此时的图：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfuncpopover.png" width = "600" height = "500" />
<br/><br/>

func函数最后一条指令：
<br/>

```
retq   // 弹出栈中内容，赋值给ip寄存器，cpu跳转执行
```
<br/>
将栈顶位置弹出，此时rsp寄存器指向的数据即为栈顶数据，看图是：main函数下一条指令（xorl）(8字节)，将它赋值给cs:ip寄存器。<br/>
>你可以认为cpu每次执行指令的地址都是从cs:ip寄存器中取的，只要改变cs:ip寄存器的值就能让cpu跳转到任何地方执行。
<br/>
当然，rsp寄存器也要像pop指令一样，上移8个字节，终于rsp寄存器重新指向了main函数的栈顶，看下此时的图：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/mainfuncret.png" width = "600" height = "500" />
<br/><br/>

此时rsp、rbp寄存器都已经恢复到调用者（main函数）的栈顶、栈底。
<br/><br/>
main函数可以继续执行了，下面我们只看一下它是如何取出的返回值就ok了。
<br/><br/>
还记得我们的返回值在哪吗？忘了的看图，在eax寄存器，36 好好地存着呢。
<br/><br/>
再来看main函数倒数第五条指令：

```
movl   %eax, -0x8(%rbp) // 将eax寄存器的值赋值给rbp寄存器往下8个字节的位置
```
将eax寄存器中的值取出，存入了main的栈内存：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/maingeteax.png" width = "600" height = "500" />
<br/><br/>

终于完成了方法的调用、返回、参数、返回值的流程，我先去寻找一会快乐，再回来写。


小总结：**计算机执行方法的方式就是通过跳转指令，然后通过寄存器与栈内存进行参数与返回值的传递，调用者将参数存入寄存器或者自己的栈内存，被调用者自己去取，而指令的格式则是：操作码+（多个）操作数**。为实现我们简陋的JVM，理解到这一句话就足够了。

>后面会实现"模版解释器"（希望我能坚持更到那一天），如果你只是想了解，那么cpu执行方法这一章节看不懂可以逃一逃，
但如果想彻底理解"模版解释器"的原理，cpu执行方法这一章节，躲不了的。当然，如果你看不懂，这也不能全赖你，我也有巨大的责任，我写的不好，请联系我。

 <br/>

  **<p id="初识执行器">3.初识JVM执行器：</p>**

  JVM全称Java虚拟机，所谓虚拟机，即是虚拟的计算机，再换句话说，它模拟了计算机执行指令的过程。

**<p id="JVM指令集">3.1 JVM指令集：</p>**


* 既然是模拟了真实的计算机，那么它必然定义了自己的指令集。<br/><br/>
* 与cpu指令集类似，JVM指令也是 **操作码+操作数** 形式。<br/><br/>
* 但是cpu能够执行指令是因为这是焊死在硬件里的，通过"译码器"可以识别，而JVM自己自定义的指令集是不能被cpu所能识别的。<br/><br/>
* 因此JVM需要自己实现逻辑去解释这些指令到底是啥意思。<br/><br/>
* 最终还是会被编译成cpu所能识别的指令集才能被cpu正确执行。<br/><br/>
* 并且，每个指令占用字节数也是有约定的。（撸代码时会详细讲述这种约定，[点我跳转](#指令解释)）

**<p id="传递">3.2 参数/返回值的传递：</p>**
  
在前文我们看到cpu执行指令与方法，无论是参数的传递还是局部变量的存储，无非就是在寄存器与栈内存之间跳来跳去，可以说**真实的计算机的指令集是基于寄存器&&栈的**。而JVM既然是虚拟的计算机，那它也必然需要实现参数的传递，才能实现方法的调用，才能像真实的计算机一样去执行方法。
有一点不同，**JVM的指令是基于栈**，它的指令实现只涉及对栈的操作。

**<p id="执行器概念">3.3 执行器概念：</p>**

  * 举一个凶猛的栗子🌰：<br/><br/>
    ACONST_NULL  1
    <br/><br/>
  这是一条JVM指令，它的意思是将NULL推向栈顶。ACONST_NULL 这个单词只是帮助我们记忆的助记符，真实值是 1 ，当JVM执行引擎读取到1的时候就会执行将"NULL推向栈顶"的逻辑，而这段逻辑的实现就需要JVM自己去实现，比如
可以用c++实现，下面是一段伪代码：
```c++

void run() {
    stack.push(NULL);
}

```

编译后：
```c++

pushq  %rbp
movq   %rsp, %rbp
subq   $0x10, %rsp
movl   $0x0, -0x4(%rbp)
leaq   0x1694a(%rip), %rdi
leaq   -0x4(%rbp), %rsi
callq  push
addq   $0x10, %rsp
popq   %rbp
retq

```

不需要看懂，你只需要知道它有好多行就可以了。

这其中调用的"push"方法的实现的编译后的汇编，我们还没有贴出来，而机器语言的入栈只需要一条指令"movl   $0x0, -0x4(%rbp)" 即可完成。
显然相比于机器语言的入栈操作，用高级语言（c++）实现的入栈的指令的数量是巨大的，这必然是会影响效率的。不过，用c++来解释JVM指令的操作正是Hotspot早期采用的方式，就是我们即将要实现的**字节码解释器**。

为了解决执行效率这个问题，Hotspot在后来的版本中采用了**模版解释器**，用动态生成机器指令的方式来解释JVM指令，即在JVM启动时，为每个JVM指令生成一段固定的机器码，取了个很吊的名字叫作"模版"，每当需要执行这条JVM指令时，
就执行这段机器码，而这段机器码的数量是远远小于用c++解析JVM指令所编译出来的机器码的数量的。

本质就是减少机器码的数量，那么执行效率就提高了。减少前叫作"字节码解释器"，减少后叫作"模版解释器"。而减少的手段，在模版解释器（此处暂时缺少超链接）我们再讲。

**<p id="方法执行理论示例">3.4 方法执行理论示例：</p>**
<br/>
举一个凶猛的栗子🌰,画点图解释一下方法调用的流程。先十分残忍地上一段代码：
```c++

public class HelloJVM {
    public static void main(String[] args) {
        TestClass t = new TestClass();
        int res = t.get(1, 2);
    }
}

public class TestClass {
    public int get(int a, int b) {
        return a + b;
    }
}

```
这段代码如果看不懂就自己把自己的腿打断。<br/>
<br/>
下面从理论层面上说一下它是如何执行的。
* 首先每一条线程都会有自己私有的一个栈空间stack，我们用下面这个黑色的矩形表示：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/ThreadStack.png" width = "250" height = "300" />

(好像麻将的白板)
<br/>
* 每当这个线程执行一个方法时就会占用一块栈内存空间，这块内存空间被取了一个很吊的名字："栈帧"
* 其实就是 new 一个栈帧，然后stack.push进去。
* 一个栈帧，首先可以作为一个小的栈使用，即可以push,pop
* 同时，还拥有一个用来存储局部变量的list。
* 我们的main方法有三个局部变量：args,t,res。所以list长度为 3:
* 好了，进入main方法了，于是我们的图变成了这个样子：
  <img src="https://github.com/ArosyW/picture/blob/master/mainStack.png" width = "600" height = "500" />
* 首先"new TestClass()"会创建TestClass对象（假如创建出的对象叫作oop）并且将其push入栈。
<img src="https://github.com/ArosyW/picture/blob/master/mainoop.png" width = "600" height = "500" />
  
* 将栈顶oop复制一份再次入栈。

* 于是我们的图变成了这个样子：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/mainoop2.png" width = "600" height = "500" />
<br/><br/>
* 然后将对象oop从栈中弹出一个，并赋值给list索引为1处，即list[1] = oop：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/mainpop1.png" width = "600" height = "500" />
<br/><br/>
接下来就是调用get(int a, int b)方法了，先解释一下为什么刚才oop在栈顶的时候被复制了一份，因为调用**非静态方法**的第一个参数默认是"调用者"，这个参数被称为this指针，不需要你写，但编译器会给你生成。在栈顶复制一份是为了后面调用
get(int a, int b)用的，t.get(1, 2)可以理解为t.get(t,1, 2)。调用**静态方法**则不会有this指针这个参数。
<br/><br/>
* 在调用get(int a, int b)之前先把需要传递的参数依次push入栈，先是参数a即1入栈：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/mainpush1.png" width = "600" height = "500" />
<br/><br/>
* 然后将参数b即2入栈：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/mainpush2.png" width = "600" height = "500" />
<br/><br/>
* 然后会生成 "被调用方法get" 的栈帧：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/getinit.png" width = "600" height = "500" />
<br/><br/>
* 并把参数写入到"被调用者"的局部变量表list，将 2 从main栈中弹出，写入get方法的局部变量表：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/getcan1.png" width = "600" height = "500" />
<br/><br/>
* 将 1 从main栈中弹出，写入get方法的局部变量表：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/getcan2.png" width = "600" height = "500" />
<br/><br/>
* 将 this指针：oop 从main栈中弹出，写入get方法的局部变量表：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/getcan3.png" width = "600" height = "500" />
<br/><br/>

**到这之后，才真正的来到get(int a, int b)，之前都是在main方法完成的。**

<br/><br/>
* get方法中需要作加法，先把加法运算的两个参数依次push入栈：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/getpushone.png" width = "600" height = "500" />
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/getpushtwo.png" width = "600" height = "500" />
<br/><br/>
* 将get栈弹出两个元素，作加法运算，然后将结果push进get栈:
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/getadd.png" width = "600" height = "500" />
<br/><br/>
* 最后该执行return了，将get栈中的3弹出，push进main方法的栈中，然后get方法到这就结束了，释放get方法的栈帧内存：
<br/><br/>
  <img src="https://github.com/ArosyW/picture/blob/master/getreturn.png" width = "600" height = "500" />
<br/><br/>

**后面就是main方法在调用完get方法后的执行流程了，看下如何取出get方法的返回值的。**

<br/><br/>
将main栈弹出3，赋值给main的局部变量表：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/maincan3.png" width = "600" height = "500" />

<br/><br/>
糊hu了！
<br/><br/>
成功完成了方法的调用与返回！
<br/><br/>
来个很diao的总结：方法调用过程中，参数的传递是**靠调用者将参数写入被调用者的局部变量**表实现的，而返回值的传递是**靠被调用者将返回值push进调用者栈**实现的。如果你仍有疑惑这些逻辑如何通过代码实现，下一章节就是。
<br/><br/>


  **<p id="字节码解释器">4.字节码解释器：</p>**

字节码解释器就是用c\c++来解释JVM指令，首先我们需要准备方法执行环境（线程、栈、栈帧等），找到JVM指令后才能开始解释并执行。

**<p id="执行环境">4.1执行环境：</p>**
   
  **本次commit :** 505b3d7e62a5eaac6e086f1f8def3934670d4b78

<br/>

[3.4方法执行理论示例](#方法执行理论示例) 讲到（3.4章节理论很重要，好好看看），每一个线程都有自己的私有的栈空间，每执行一个方法就会创建一个栈帧push入栈，执行完成再pop出栈。
显然我们首先要有一个来描述线程的C++类JavaThread,属性是一个栈：

```c++
class JavaThread {
    stack<JavaVFrame*> stack; // 线程栈空间
};

```

而栈里面所存的，应该是栈桢JavaVFrame，还记得栈桢里面有啥吗？栈桢本身可以支持栈的操作（push、pop）,还要有一个局部变量表list：

```c++
class JavaVFrame {
    stack<char*> stack; // 方法的栈空间
    map<int,char*> locals; // 方法的局部变量表
};

```
我们的main方法是一个static方法，于是我们可以建一个callStaticMethod的方法，专门用来调用static方法，这样就不局限于main方法了：

```c++
void JavaNativeInterface::callStaticMethod(JavaThread* javaThread, MethodInfo *method) {
    printf("===============执行方法 :%s =================\n", method->getMethodName().c_str());
    JavaVFrame *javaVFrame = new JavaVFrame;//马上要执行方法了，先创建栈帧
    javaThread->stack.push(javaVFrame);//栈帧push进线程的栈空间
//    BytecodeInterpreter::run(curThread, method); // 执行方法
    javaThread->stack.pop();//将执行完成的栈桢弹出栈空间
    delete javaVFrame; //释放栈桢内存空间
}
```

你会愤怒地发现其中BytecodeInterpreter::run方法我们还没有创建，这就是我们真正解释JVM指令的地方，下一小节实现。
<br/><br/>
本章节先来一个测试，还有一个目的就是看下新增的这些类是如何使用的：

```c++
int main() {
    string name = "HelloJVM";
    InstanceKlass *klass = BootClassLoader::loadKlass(name);//加载HelloJVM类
    MethodInfo *m = JavaNativeInterface::getMethod(klass, "main", "([Ljava/lang/String;)V");//遍历klass所有的方法，找到main方法。getMethod就是一个循环，没有贴出，请去代码中查看。
    JavaThread *javaThread = new JavaThread;//模拟线程的创建
    JavaNativeInterface::callStaticMethod(javaThread,m);//执行main方法
    return 0;
}
```
<br/>

输出：<br/>

===============执行方法 :main =================

小总结：我们成功地创建了方法执行所需要的线程、栈、栈帧，就差将方法中的JVM指令取出来解释它了！

**<p id="指令解释">4.2指令解释：</p>**

在指令在解释的实现过程中我们会优先实现我们的HelloJVM用到的：getstatic、

**<p id="nop">4.2.1 NOP指令的实现：</p>**

**本次commit :** 0028cd2c8ebcd5ccabd391f1f89ab4d2a07001cb

<br/>

JVM指令格式：

| 操作码  | 操作数  | 
| ----  | ----  | 
|    1字节 |  \>\=0字节 |  

操作码只占一个字节，意味着JVM指令最多有256条指令，实际上已经定义了200多条。每一个操作码都**约定**好了后面跟多少个字节的操作数，而这种约定在Hotspot源码中的Bytecodes::initialize()中详细定义了，
取两个举一个栗子🌰：

| name  | 格式  | 16进制
| ----  | ----  | ----  |
|    nop |  b | 0 | 
|    iload |  bi | 0x15 |
|    bipush |  bc | 0x10 |

<br/><br/>
**其中格式，假设字母数量为length,就表示这条指令所占字节总数为length。**
<br/><br/>
>一个栗子🌰：
先读取一个字节，发现是0，则表示是nop指令，
它的格式是b，只有一个英文字母，意味着这条指令只占一个字节，而这一个字节就是我们已经读取到的0,那么这一条指令就到此为止了。

<br/><br/>

>两个栗子🌰：
先读取一个字节，发现是0x15，则表示是iload指令，
它的格式是bi，有2个英文字母，意味着这条指令占2个字节，而第一个字节就是我们已经读取到的0x15,还要再读取一个字节，然后把这个字节推向栈顶。

<br/><br/>
其中b、bi、bc，还会有'bk'等，解释一下这些字母的含义：

| 字母  | 意义  | 
| ----  | ----  | 
|    b |  这条指令的长度是不可变的 | 
|    c |  操作数为有符号的常量 | 
|    i |  无符号的本地变量表索引 | 
|    j |  常量池缓存索引 | 
|    k |  无符号常量池索引 | 
|    o |  分支偏移 | 
|    _ |  可忽略 | 
|    w |  可用来扩展局部变量表索引的字节码 | 

我只是随便贴一贴，显得很全面，这些东西硬生生的去理解自然是困难的，我们还是一条一条指令地来实现吧，莫慌，大部分指令三五行代码就结束了。
<br/><br/>
Dont talk,show you my code！

先把上一小节缺失的JVM指令执行方法BytecodeInterpreter::run 补一下：

```c++
bool BytecodeInterpreter::run(JavaThread *javaThread, MethodInfo *methodInfo) {
    //取出字节码指令
    BytecodeStream *bytecodeStream = methodInfo->getAttributeInfo()->getCode();
    int index = 0;
    while (index < bytecodeStream->getLength()) {
        unsigned char c = bytecodeStream->readByOne(index);
        printf("指令字节：%X\n", c);
        //todo 对操作码c进行解释
    }
    printf("===============执行方法结束 :%s =================\n", methodInfo->getMethodName().c_str());
    return true;
};
```
<br/><br/>
我们在类加载的时候已经把JVM指令存储在每个方法的CodeAttributeInfo属性中了，get一下就能取到，然后就一个字节一个字节的读就完事了。
上面代码中第一次读取到的c就是我们这个方法的第一条字节码指令，假如这个c==0, 那么就是我们的nop指令，可惜不等于0。
<br/><br/>

上面代码中的todo应该怎么写呢？第一反应肯定是switch case，类似于：

```c++
    while (index < bytecodeStream->getLength()) {
        unsigned char c = bytecodeStream->readByOne(index);
        printf("指令字节：%X\n", c);
        switch (c) {
            case 0:
                printf("执行nop指令\n");
                break;
            case 1:
                ……
                break;
        }
    }
```
但是JVM有两百多个指令，我无法忍受要写两百多个case。所以这里我们稍微骚操作一下，这些指令无非就是0到256个数字，那么我们可以用一个数组，这个数组里面存的是每一条
指令的解释逻辑封装成的方法，而索引就是这0到256个数字，在c\c++中是可以做到的，于是我们新建一个真正解释JVM指令的c++类：

```c++
CODERUN CodeRunBase::run[256];
void CodeRunBase::initCodeRun(){
    run[NOP] = funcNOP; // NOP枚举值为0
}
void CodeRunBase::funcNOP(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
//nop指令什么也不做
}
```

于是在BytecodeInterpreter::run中我们可以这样去使用它：

```c++
bool BytecodeInterpreter::run(JavaThread *javaThread, MethodInfo *methodInfo) {
    //取出字节码指令
    BytecodeStream *bytecodeStream = methodInfo->getAttributeInfo()->getCode();
    int index = 0;
    while (index < bytecodeStream->getLength()) {
        unsigned char c = bytecodeStream->readByOne(index);
        printf("指令字节：%X\n", c);
        CodeRunBase::run[c](javaThread, bytecodeStream, index); //解释指令
    }
    printf("===============执行方法结束 :%s =================\n", methodInfo->getMethodName().c_str());
    return true;
};
```
来做一个测试（main方法本小节无改动）：

```c++
int main() {
    string name = "HelloJVM";
    InstanceKlass *klass = BootClassLoader::loadKlass(name);//加载HelloJVM类
    MethodInfo *m = JavaNativeInterface::getMethod(klass, "main", "([Ljava/lang/String;)V");//遍历klass所有的方法，找到main方法
    JavaThread *javaThread = new JavaThread;//模拟线程的创建
    JavaNativeInterface::callStaticMethod(javaThread,m);//执行main方法
    return 0;
}
```
<br/>

输出：<br/>

>===============执行方法开始 :main =================
<br/>
指令字节：B2
<br/>指令字节：0
<br/>指令字节：2
<br/>指令字节：4
<br/>指令字节：B6
<br/>指令字节：0
<br/>指令字节：3
<br/>指令字节：B1
<br/>===============执行方法结束 :main =================

<br/>其实只有 "指令字节：B2" 是正确的，因为我们还没有实现解释B2指令的逻辑，所以导致后面的指令都是错乱的，而B2
所对应的指令是GetStatic指令，接下来就整它！

小总结：本章节说了一大堆，其实就是把JVM指令解释的逻辑封装成一个个方法，放进数组，然后实现了一下nop指令，算是举了一个栗子🌰。

**<p id="getstatic">4.2.2 getstatic指令的实现：</p>**

**本次commit :** 21f5b84755df4520f6e806deb343555c5fa52c59

<br/>
getstatic 指令的格式：
<br/>

| name  | 操作码  | 操作数
| ----  | ----  | ----  |
|    getstatic |  1字节 | 2字节 |

<br/>
意思是从某个Java类中获取它的一个静态变量的值，push进当前方法的栈顶，伪代码：

```c++
    var value = getByClass(name);
    stack.push(value);
```
为了更好的存储变量，我们抽象一个C++类专门用来描述变量的值：

```c++
class CommonValue {
    int type ;//值的类型：int、byte、float……
    char *val;//值
};
```

<br/>
可是类的静态变量我们还没有地方来存储，是不可能获取到的，所以我们先在InstanceKlass中新增一个map用来存储类的静态变量：
<br/>

```c++
class InstanceKlass {
    int magic; //魔数，CAFEBABE:用来校验是否是.class文件
    short minorVersion; //JDK次版本号
    …………
    map<string, CommonValue*> staticValue; //静态变量 （新增）
}
```

还需要两个方法用来通过"操作数"，从Class常量池中获取到类名、属性名，由于是从Class常量池中获取，所以我们把它新增在ConstantPool类中：

```c++
string ConstantPool::getClassNameByFieldInfo(unsigned short index){
    int i = htonl(*(int *)data[index]); //取出索引
    int classIndex = i >> 16; // 取出i的左16位
    //获取class全限定名
    int classNameIndex = *data[classIndex]; //取出类名的索引
    return (data[classNameIndex]); // 返回类名
};

string ConstantPool::getFieldName(unsigned short index){
    int i = htonl(*(int *)data[index]);//取出索引
    int ind = i & 0xFF; // 取出i的右16位
    int nameAndType = htonl(*(int *)data[ind]); // 取出nameAndType的索引
    int t = nameAndType >> 16; // 取nameAndType的索引 的左16位
    return (data[t]); // 返回变量名
};
    
```
看懵了吗？你只需要知道这些属性都是我们在解析常量池的时候存进去的，现在取出来就可以了。可以回去看一下我们是怎么存的file、method、Interface Methodref。

于是我们的getstatic方法就出来了:
```c++
void CodeRunBase::funcGETSTATIC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    *执行指令GETSTATIC\n");
    unsigned short opera = bytecodeStream->readByTwo(index); //取出操作数
    printf("\t\tconstantPool index is:%d\n", opera);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByFieldInfo(opera); // 根据操作数 从 常量池获取类名
    string fieldName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getFieldName(opera);// 根据操作数 从 常量池获取变量名
    printf("\t\tclassName:%s,fieldName:%s\n", className.c_str(), fieldName.c_str());
    InstanceKlass *klass = BootClassLoader::loadKlass(className); //根据类名取得加载完的klass对象
    CommonValue *commonValue = klass->staticValue[fieldName]; // 根据变量名取出 值
    javaThread->stack.top()->stack.push(new CommonValue(commonValue->type, commonValue->val)); //将前面取出的值推向栈顶
}
```

<br/>

来做一个测试（main方法本小节无改动）：

```c++
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
```
<br/>

输出：<br/>
>===============执行方法开始 :main =================<br/>
>指令字节：B2<br/>
>    *执行指令GETSTATIC<br/>
>    constantPool index is:2<br/>
>    className:java/lang/System,fieldName:out

由于我们目前还不能正确地加载java/lang/System类，所以我们的方法在执行
```c++
    InstanceKlass *klass = BootClassLoader::loadKlass(className); //根据类名取得加载完的klass对象
```
时就执行不下去了。我们先把我们需要的指令的逻辑写完，后面再处理这些问题。

小总结：我们通过getstatic指令的操作数从Class常量池中获得了getstatic指令所需要的 类名、变量名，并把变量的值推向了栈顶。

**<p id="iconst0">4.2.3 iconst0指令的实现：</p>**

**本次commit :** ba117068ad7af8bfc726a03fcbb9280e60fcb982

<br/>

>为了给你增强信心，所以特意在这先实现一个简单的命令。

<br/><br/>
iconst0 的意思是将int类型的0推向栈顶：

```c++

void CodeRunBase::funcICONST0(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    javaThread->stack.top()->stack.push(new CommonValue(T_INT, 0));
}

```

是的这条指令就一行代码就完成了，事实上很多指令都是这样的，等我们实现完我们本次需要使用到的指令时，再来实现其他的指令。是不是瞬间又有了信心！

**<p id="putstatic">4.2.4 putstatic指令的实现：</p>**

**本次commit :** 8c55bdbb88d015dfe6462de0bd2ca24ec3d00b49

<br/>

putstatic 指令的格式：
<br/>

| name  | 操作码  | 操作数
| ----  | ----  | ----  |
|    putstatic |  1字节 | 2字节 |

刚好与getstatic指令相反，getstatic指令是从类中取出静态变量推向栈顶，putstatic是从栈顶弹出一个静态变量写入类中：

```c++

void CodeRunBase::funcPUTSTATIC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令PUTSTATIC\n");
    unsigned short opera = bytecodeStream->readByTwo(index);//取出操作数
    printf("\t\tconstantPool index is:%d\n", opera);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByFieldInfo(opera);// 根据操作数 从 常量池获取类名
    string fieldName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getFieldName(opera);// 根据操作数 从 常量池获取变量名
    printf("\t\tclassName:%s,fieldName:%s\n", className.c_str(), fieldName.c_str());
    CommonValue* value =javaThread->stack.top()->pop(); //弹出栈顶变量
    InstanceKlass *klass = BootClassLoader::loadKlass(className);//根据类名取得加载完的klass对象
    klass->staticValue[fieldName] = value; //将从栈顶获得的静态变量写入类
}

```
**<p id="ldc">4.2.5 ldc指令的实现：</p>**

**本次commit :** 03c113485ab72a91b6cd57f4da46fe2752f1614a

<br/>

ldc指令格式：

| name  | 操作码  | 操作数
| ----  | ----  | ----  |
|    ldc |  1字节 | 1字节 |

这一个字节的操作数是指Class常量池的索引，意思是将一个常量池的值推向栈顶。
<br/><br/>
当然，这个值是需要区分类型的，还记得常量池的类型存储在哪里吗？在ConstantPool中的tag属性中。我们这里只用到了String，于是我们这样来解释ldc指令：

```c++

void CodeRunBase::funcLDC(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令LDC\n");
    unsigned char opera = bytecodeStream->readByOne(index);//取出操作数
    unsigned char tag = *(bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->tag + opera);//从常量池的类型中取出本次ldc指令操作的数据类型
    switch (tag) {
        case CONSTANT_Float:
            break;
        case CONSTANT_String: {
            int index = *bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->data[opera]; //从常量池获取索引
            string str = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getStringFromPool(index);//从常量池获取string的内容
            javaThread->stack.top()->stack.push(new CommonValue(T_OBJECT, (char *)str.c_str()));//入栈
            break;
        }
        case CONSTANT_Class:
            break;
        default:
            printf("未知类型\n");
    }
}

```

**<p id="dup">4.2.6 dup指令的实现：</p>**

**本次commit :** 1501b636441d8700d3914cd48fb6877e6cef4a7f

<br/>

这条指令也很简单，把栈顶的值复制一份推向栈顶就完事，于是我们这样来解释dup指令：

```c++
void CodeRunBase::funcDUP(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    CommonValue *cv = javaThread->stack.top()->stack.top(); //获取栈顶值
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}
```

**<p id="aload0">4.2.7 aload0、aload1指令的实现：</p>**

**本次commit :** 14093f6fca183040270763f8be7ac32e6364991a

<br/>

load相关的指令都是从局部变量表中获取一个变量，推向栈顶，这次我们要实现的aload0，其中a表示我们的要操作的数据类型为引用类型，0 表示局部变量表第一个数据，当然还有aload1、iload0……
<br/><br/>于是我们这样来解释aload0、aload1指令：

```c++
void CodeRunBase::funcALOAD0(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令aload0\n");
    CommonValue *cv = javaThread->stack.top()->locals[0];//获取局部变量表第一个数据
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}

void CodeRunBase::funcALOAD1(JavaThread *javaThread, BytecodeStream *bytecodeStream , int& index) {
    printf("    **执行指令aload1\n");
    CommonValue *cv = javaThread->stack.top()->locals[1];//获取局部变量表第一个数据
    javaThread->stack.top()->stack.push(cv); // 推向栈顶
}
```

**<p id="new">4.2.8 new指令的实现：</p>**

**本次commit :** 88cbc04665c1a22e84f93a47a73a15b414755ca9

<br/>

new指令格式：

| name  | 操作码  | 操作数
| ----  | ----  | ----  |
|    new |  1字节 | 2字节 |

我们每次new对象都会有这个指令的执行，这个指令2字节大小的操作数是Class常量池索引，通过这个索引我们可以拿到一个Java类的名字（全限定名）。

**那么这个指令的实现逻辑就是将这个Java类实例化（创建对象）,然后将这个对象push入栈。**

既然需要创建对象，那么一定要先用C++把对象表示出来。

之前讲到所有的Java类在JVM中都会有一个Klass"一一对应"，于是我们新建了C++类InstanceKlass来表示Java类。

大家都知道，在写Java代码时可以通过一个Java类new出很多对象，事实上，在JVM中我们也需要一个C++类来表示Java对象：InstanceOop

InstanceOop应该有哪些属性？

* 为了将一个Java对象与实例化它的Java类关联起来，显然InstanceOop中应该有指向所属Java类的指针。

* 每创建一个对象，Java类的非静态成员属性就会被复制一份，让这个属性专属于对象，显然InstanceOop中应该有所属Java类的成员属性。

>事实上，在成熟的JVM中还会有对象头等信息，对象头里面存储了例如锁等相关的信息，但鉴于我们的JVM暂不需要，为了快速实现，我们就不加对象头了，不排除后面用到再补。

于是我们新建InstanceOop：

```c++

class InstanceOop {
    InstanceKlass *klass;//所属类

    map<string, char *> map;//实例化的成员属性
};

```

那么应该如何创建一个对象呢？

在Hotspot中，创建一个对象的步骤是较为繁多的，
* 计算这个对象所需要的（内存对齐后的）内存大小
* 分配内存空间
* 初始化所属类等信息
* 执行类的构造方法

内存对齐、内存分配 等内存相关的内容我们会第三部分（超链接）讲，所以此处我们伪实现一个即可。

在InstanceKlass中新建allocateInstance方法专门用来实例化对象：

```c++

InstanceOop* InstanceKlass::allocateInstance(InstanceKlass* klass) {
    InstanceOop *oop = new InstanceOop;
    oop->klass = klass;
    return oop;
}

```

你会发现我们并没有执行构造方法，因为执行构造方法并不在new指令中完成，而是会在new指令之后，通过 invokespecial 指令实现构造方法的调用。

我们先来实现new指令：

```c++

void CodeRunBase::funcNEW(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令NEW\n");
    unsigned short opera = bytecodeStream->readByTwo(index);
    string classPath = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassPath(opera);
    InstanceKlass *klass = BootClassLoader::loadKlass(classPath);
    InstanceOop *oop = InstanceKlass::allocateInstance(klass);
    javaThread->stack.top()->stack.push(new CommonValue(T_NARROWOOP, (char *) oop));
}

```
**<p id="invokevirtual">4.2.9 invokevirtual指令的实现：</p>**

**本次commit :** edeb60b3bb8f6ad295ed217e3441189b88c8eb0e


JVM调用方法的指令主要有四个invokevirtual、invokeinterface、invokespecial、invokestatic。

>第五个：invokedynamic

* invokestatic ： 调用类的静态方法
* invokespecial ： 调用特殊方法，例如构造方法、私有方法、父类方法、本地方法。
* invokevirtual ：调用对象能够调用的实例方法。
* invokeinterface ： 调用接口方法，需要搜索并找到合适的方法实现，然后调用。

这里我先来实现invokevirtual方法，下面给出它的指令格式：


| name  | 操作码  | 操作数
| ----  | ----  | ----  |
|    invokevirtual |  1字节 | 2字节 |

<br/>

根据这2个字节的操作数，根据它可以从Class常量池中获取到需要调用的方法的Java类名、方法名字、方法描述。
其中Java类名、方法名字很好理解，重点说一下'方法描述'，它描述了这个方法的参数与返回值（包括类型与数量），必须举一个栗子🌰：


(Ljava/lang/String;)V

假如一个方法的描述是上面这个字符串，那么括号里面的即为参数，括号外面的 V 即为返回值，也就是指返回值为void。

再来看括号里面的参数，每个参数之间用 '；' 分隔（除基本类型的数组外），显然我们只有一个参数，开头的L表示这个参数的类型为某个Object，也就是java/lang/String这个类型。

于是我们可以把这个'方法描述'理解为：

| 参数数量  | 入参数  | 返回值
| ----  | ----  | ----  |
|    1 |  String | void |

这里再给出'方法描述'中的其他部分符号的含义：

| 符号  | Java含义 
| ----  | ----  | 
|    Z |  Boolean | 
|    B |  Byte | 
|    C |  Char | 
|    S |  Short | 
|    I |  Int | 
|    J |  Long | 
|    F |  Float | 
|    D |  Double | 
|    V |  Void | 
|    [ |  数组 | 

考虑到篇幅，更详细的规则不再给出，你只需要知道从'方法描述'中我们可以解析出这个方法的参数与返回值即可。当你真的用到更多的规则时再查阅更多的资料吧。

[3.4方法执行理论示例](#方法执行理论示例)  下面的实现逻辑与这一章节有很大的关系，忘记了的记得复习一下。

先简要列出一下invokevirtual的实现逻辑：

* 根据操作数从Class常量池中获取到需要调用的方法的Java类名、方法名字、方法描述
* 解析方法入参出参、方法数量
* 创建 "被调用者方法的栈帧frame"
* 将需要的参数写入 "调用者方法的栈帧frame的局部变量表"
* 执行 "被调用者方法"

还记得我们如何调用的main方法吗，我们写了一个callStaticMethod 方法专门调用static方法，现在我们在来写一个专门给invokevirtual指令使用的callVirtual：
```c++
void JavaNativeInterface::callVirtual(JavaThread* javaThread, MethodInfo *method,int paramCount ,char** params) {
    printf("===============执行方法开始 :%s =================\n", method->getMethodName().c_str());
    JavaVFrame *javaVFrame = new JavaVFrame;//马上要执行方法了，先创建栈帧
    for (int i = 1; i <= paramCount; ++i) { //将参数写入局部变量表
        javaVFrame->locals[i] = (CommonValue *) (*(params + i - 1));
    }
    javaVFrame->locals[0] = (CommonValue *)*(params + paramCount);//非静态方法第一个参数为this指针
    javaThread->stack.push(javaVFrame);//栈帧push进线程的栈空间
    BytecodeInterpreter::run(javaThread, method); // 执行方法
    javaThread->stack.pop();//将执行完成的栈桢弹出栈空间
    delete javaVFrame; //释放栈桢内存空间
}
```
那么我们的invokevirtual指令的实现逻辑只需要把callVirtual需要的参数准备好就可以了：

```c++

void CodeRunBase::funcINVOKEVIRTUAL(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令INVOKEVIRTUAL\n");
    unsigned short opera = bytecodeStream->readByTwo(index);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByMethodInfo(opera);//获取类名
    string methodName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getMethodNameByMethodInfo(opera);//获取方法名
    string descName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getDescriptorNameByMethodInfo(opera);//获取方法描述
    printf("\tclassName:%s,methodName:%s,descName:%s\n", className.c_str(), methodName.c_str(),
           descName.c_str());
    int paramCount =0 ;//初始化参数数量
    char **params = CodeRunBase::getParams(descName, javaThread->stack.top(),paramCount);//解析参数
    InstanceKlass *klass = BootClassLoader::loadKlass(className);//获取类全限定名
    MethodInfo *m = JavaNativeInterface::getMethod(klass, methodName, descName);//根据方法名字和方法描述找到要调用的方法
    JavaNativeInterface::callVirtual(javaThread, m, paramCount, params);//调用方法
}
```
**<p id="invokespecial">4.2.10 invokespecial指令的实现：</p>**

**本次commit :** 9c397b1f49fda967c59f1ca80797b1084c7868c3

invokespecial 主要用来调用特殊方法，例如构造方法、私有方法、父类方法、本地方法。我们这里用到了构造方法和本地方法，所以我们优先来实现这两个方法的调用逻辑。

首先它的指令格式与invokevirtual完全一样：

| name  | 操作码  | 操作数
| ----  | ----  | ----  |
|    invokespecial |  1字节 | 2字节 |

<br/>

实现逻辑也与invokevirtual类似（认真看一下上一小节invokevirtual的实现，这里不细展开了），只不过要单独处理一下本地方法的调用（我们在下一小节再实现本地方法）：

```c++
void JavaNativeInterface::callSpecial(JavaThread* javaThread, MethodInfo *method,int paramCount ,char** params) {
    printf("===============执行方法开始 :%s =================\n", method->getMethodName().c_str());
    JavaVFrame *javaVFrame = new JavaVFrame;//马上要执行方法了，先创建栈帧
    for (int i = 1; i <= paramCount; ++i) { //将参数写入局部变量表
        javaVFrame->locals[i] = (CommonValue *) (*(params + i - 1));
    }
    javaVFrame->locals[0] = (CommonValue *)*(params + paramCount);//非静态方法第一个参数为this指针
    javaThread->stack.push(javaVFrame);//栈帧push进线程的栈空间
    BytecodeInterpreter::run(javaThread, method); // 执行方法
    javaThread->stack.pop();//将执行完成的栈桢弹出栈空间
    delete javaVFrame; //释放栈桢内存空间
}
```

```c++

void CodeRunBase::funcINVOKESPECIAL(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令INVOKESPECIAL\n");
    unsigned short opera = bytecodeStream->readByTwo(index);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByMethodInfo(opera);//获取类名
    string methodName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getMethodNameByMethodInfo(opera);//获取方法名
    string descName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getDescriptorNameByMethodInfo(opera);//获取方法描述
    printf("\tclassName:%s,methodName:%s,descName:%s\n", className.c_str(), methodName.c_str(),
    descName.c_str());
    int paramCount =0 ;//初始化参数数量
    char **params = CodeRunBase::getParams(descName, javaThread->stack.top(),paramCount);//解析参数
    InstanceKlass *klass = BootClassLoader::loadKlass(className);//获取类全限定名
    MethodInfo *m = JavaNativeInterface::getMethod(klass, methodName, descName);//根据方法名字和方法描述找到要调用的方法
    // todo 如果是本地方法则调用本地方法
JavaNativeInterface::callSpecial(javaThread, m, paramCount, params);//调用方法
}
```

**<p id="本地方法">4.2.11 本地方法的调用：</p>**

**本次commit :** 1075a48dfea564e08beecc7b693abaf2713e0b3f


Java虚拟机规范并没有规定本地方法调用的实现必须怎样，所以我们是自由的。这里我们用一个map把所有的本地方法存放起来，key即为方法名字，value为方法的实现，在JVM启动时初始化并填充这个map，
于是我们新建C++类CodeRunNative来存放本地方法：
```c++
typedef void (*CODENATIVE)(int paramsCount, char **params);

class CodeRunNative {
public:
    static void initCodeRun();
    static map<string, CODENATIVE> map;
};
```
其中CODENATIVE是一个函数指针，你可以理解在填充map时可以将它指向方法的实现。

其中initCodeRun则是我们在JVM启动时调用的，它会填充我们的map，将本地方法的实现全部put进去。

假如我们有一个本地方法叫做 writeBytes ，那么我们可以这样来填充：

```c++
void CodeRunNative::initCodeRun() {
    map["writeBytes"] = writeBytes;
}

void CodeRunNative::writeBytes(int paramsCount, char **params) {
    //todo 本地方法的实现
};

```

那么我们可以补充一下invokespecial指令未实现的本地方法的逻辑了，就像如何把大象装进冰箱一样简单，这里我们只需要两步：

* 判断出这是一个本地方法
* 调用这个本地方法

先说如何判断出这是一个本地方法，之前我们在解析字节码文件的时候， 解析“方法”时将这个方法的访问权限存放在了MethodInfo->accessFlags中，再贴一下这个属性的含义：

| 标志  | 二进制值  | 
| ----  | ----  | 
|    public |  0000000000001 | 
|    private |  0000000000010 | 
|    protected |  0000000000100 | 
|    static |  0000000001000 | 
|    final |  0000000010000 | 
|    synchronized |  0000000100000 | 
|    bridge |  0000001000000 | 
|    varargs |  0000010000000 | 
|    native |  0000100000000 | 
|    abstract |  0010000000000 | 
|    strict |  0100000000000 | 
|    synthetic |  1000000000000 | 

那么只需要位运算一下 （ accessFlags&100000000 == 100000000 ） 就可以判断出这个方法是native方法。

我们来完善一下invokespecial指令本地方法的调用：

```c++

void CodeRunBase::funcINVOKESPECIAL(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
    printf("    **执行指令INVOKESPECIAL\n");
    unsigned short opera = bytecodeStream->readByTwo(index);
    string className = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getClassNameByMethodInfo(opera);//获取类名
    string methodName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getMethodNameByMethodInfo(opera);//获取方法名
    string descName = bytecodeStream->getBelongMethod()->getBelongKlass()->getConstantPool()->getDescriptorNameByMethodInfo(opera);//获取方法描述
    printf("\tclassName:%s,methodName:%s,descName:%s\n", className.c_str(), methodName.c_str(),
           descName.c_str());
    int paramCount =0 ;//初始化参数数量
    char **params = CodeRunBase::getParams(descName, javaThread->stack.top(),paramCount);//解析参数
    InstanceKlass *klass = BootClassLoader::loadKlass(className);//获取类全限定名
    MethodInfo *m = JavaNativeInterface::getMethod(klass, methodName, descName);//根据方法名字和方法描述找到要调用的方法
    if (m->getAccessFlags() & 100000000 == 0x0100) {//判断是否是本地方法
        FUNNATIVE nativeFunc = (FUNNATIVE) CodeRunNative::map[m->getMethodName()]; //取出本地方法
        nativeFunc(paramCount,params);//调用本地方法
        return;
    }
    JavaNativeInterface::callSpecial(javaThread, m, paramCount, params);//调用方法
}

```

**<p id="return">4.2.12 return指令的实现：</p>**

**本次commit :** 36d311ad7358460969a67a1fba6a7e1e0b38e979

编译器会给每一个Java方法生成一个return系列的指令，即使返回值为void，即使你并没有显式的将"return"写出，在方法结束时仍然会有return相关的指令，下面给出return相关的指令以及含义：

| 指令  | 含义  |
| ----  | ----  |
|    ireturn |  返回一个int类型的数据 |
|    lreturn |  返回一个long类型的数据 |
|    freturn |  返回一个float类型的数据 |
|    dreturn |  返回一个double类型的数据 |
|    areturn |  返回一个引用类型的数据 |
|    return |  啥也不返回（void） |

用ireturn举例，其实现逻辑为：

* 弹出当前方法的栈顶元素
* 将其push进 当前方法的调用者的栈顶

由于我们的字节码解释器并没有保存调用者的栈地址，因此就不实现ireturn了，直接实现return就足够现阶段的我们使用了，return指令什么都不需要做：

```c++

void CodeRunBase::funcRETURN(JavaThread *javaThread, BytecodeStream *bytecodeStream, int &index) {
//nothing to do .
    printf("    **执行指令RETURN\n");
}

```

>模版解释器才是JVM的精髓所在，所以在模版解释器我们一定会做保存调用者栈基地址这些操作。


**<p id="打印">4.3 终于完成Hello JVM 的打印！：</p>**

**<p id="JDK">4.3.1 自定义JDK的准备：</p>**

**本次commit :** https://github.com/ArosyW/OurJDK

由于Hotspot提供的jdk对于我们现阶段自己实现的这个简陋的JVM来说，过于复杂，我们暂时无法对其进行完全的类加载（例如接口、父类、异常等我们暂未实现）。所以我们不得不自己手写属于我们自己的JDK,
别担心，都是Java代码，非常简单，就是定义几个Java类，定义几个方法而已。
<br/><br/>
我们自己实现JDk自然和Hotspot有所区别，为了避免对你产生误导，这里先说明以下Hotspot是如何实现：

```java

System.out.println("Hello JVM")；

```
首先System中有一个静态属性out:
```java

public final static PrintStream out = null;

```
out被初始化为了null，你会愤怒的发现你找不到它何时被赋值的。因为它是JVM在C++代码的帮助下调用System.initializeSystemClass实现初始化的。
<br/><br/>
* 为了"简陋"，我们自定义JDK将不选择这么做，我们直接new就ok
* 显然我们需要定义一个System类（java.lang.System），里面有一个静态属性out，类型为PrintStream
* 显然我们还需要定义一个PrintStream（java.io.PrintStream）类用来声明out
* 显然PrintStream类里面应该有一个方法println用来供我们调用，它将实现打印的功能

于是我们的包结构变成了这个样子：

<img src="https://github.com/ArosyW/picture/blob/master/pack.jpeg" width = "320" height = "250" />

（有报红没关系，是编译器识别到的不是我们自定义的JDK引起的）

其中：
```java

public class System {
    public final static PrintStream out = new PrintStream();
}

```

```java

public class PrintStream {
    public void println(String x) {
        write0(x);
    }
    private native void write0(String x);
}

```
我们最终通过本地方法write0调用到我们的JVM上的C++代码来实现打印，自定义JDK我已经上传至github了，另外，如果你因为System类中报红而无法编译，你只需要将你的out文件删除重新运行即可。


**<p id="成功">4.3.2 成功打印：</p>**

**本次commit :** 764c52f125f59e0f8532d43768f39809e3eb0c53

* 作为打印"Hello JVM"的第一阶段的最后一节，这一小节主要修修补补前面的漏洞（写的多了，难免有不合适的地方，见谅），首先要实现native方法"write0"，我们才能成功打印：
```c++

void CodeRunNative::writeBytes(int paramsCount, char **params) {
    CommonValue *cv = (CommonValue*)*params; //目前只有一个参数 ，就直接取了
    printf("执行本地方法write0,以下为输出结果：\n");
    printf("%s\n",cv->val); //打印参数的值
};

```
* 新建了Threads.cpp用来记录当前java线程，以方便随时可以取到。

* BootClassLoader::loadKlass中修复了以下字符串append相关的问题，不细阐述了。

记得把BootClassLoader.cpp中的路径更换为你编译好的项目路径。

输出结果：

```

…………

执行本地方法write0,以下为输出结果：
HelloJVM
指令字节：B1
    **执行指令RETURN
===============执行方法结束 :println =================
指令字节：B1
    **执行指令RETURN
===============执行方法结束 :main =================

```



  **<p id="模版解释器">5.模版解释器：</p>**

在看整个模版解释器所有的内容之前，请务必将 [cpu是如何执行指令的](#cpu是如何执行指令的)  [cpu是如何执行方法的](#cpu是如何执行方法的)  
<br/>这两小节彻底搞懂，否则模版解释器你将痛苦万分（当然，另一个不痛苦的方式就是，不看，不学）
<br/>

>PS:必须说明一下的是，整个模版解释器提供的代码量是极少的，两个原因：
> 一、时间上，大部分都是机器指令的编写，工作量远远超出了我的预期。
> 二、意义上，相信来此的大部分是学习，而不是真的要实现可用的JVM，而模版解释器又涉及跨平台与编译器的问题，导致我个人编写出的模版解释器将与Hotspot有较大差距，读者若真的有时间研究我的机器指令的设计，倒不如直接研究Hotspot在
> 这一部分的设计更有意义。
> 综上：模版解释器将作为你研究Hotspot的入门引导，我就不上大量代码献丑了（CallStub会提供代码）。

**<p id="初步认识">5.1初步认识：</p>**

在前面字节码解释器中我们可以看到JVM定义了属于自己的指令集，但cpu是无法识别这些指令的，需要JVM自己用代码去解释这些指令，很显然，Java可以说是一门解释型的语言。
这样就有一个问题，就是在解释这些JVM指令的时候用的是C++的代码，最终会被编译成大量的机器指令，这个“大量”，严重影响了Java的执行效率。（严重到被人鄙视哈哈）
<br/><br/>

>那么如何提升执行效率呢？

<br/>
很简单，既然因为被编译成的机器指令量太大，那么我们想办法减少机器指令的数量就可以了嘛。
<br/><br/>

>如何减少指令数量？

<br/> 
看起来再用c++来解释这些JVM指令行不通了，因为c++会被编译器按规则编译成固定的机器指令，无法减少其数量。很明显，只有一条路可以走，就是手动设计机器指令的逻辑。
<br/><br/>
你可以理解为用c++调用我们自己编写的机器指令。
<br/><br/>

>p如何用c++调用我们自己编写的机器指令呢？

 * 内嵌汇编代码 （你可以在搜索引擎找到你想要的任何相关资料，因为并没有采用它，就不展开了）
 * 函数指针！

函数指针，可以将一个指针指向某个方法，我们在字节码解释器中用到过，例如将它指向Native方法。
<br/><br/>
但我们之前的用法是将它指向一个c++的方法。
<br/><br/>
让我们重新思考，一个方法无非就是一段连续的内存空间，其中存放了一些二进制数，这些数被cpu执行时就可以理解为机器指令。
<br/><br/>
那假如我们手动向某块连续的内存空间中写入我们自定义的二进制数，然后，把提前声明好的函数指针，指向这块连续的内存的首地址。
<br/><br/>
岂不是，就可以实现“自定义机器指令的调用”！
<br/><br/>
这种天才的想法正是Hotspot所采用的。
<br/><br/>
必须举一个严肃的栗子🌰：

```c++

typedef int (*Incr)(int arg);//类型：函数指针
int main() {
    int code[] = {0x55,0x48,0x89,0xe5,0x89,0x7d,0xfc,0x8b,0x45,0xfc,0x83,0xc0,0x01,0x89,0x45,0xfc,0x5d,0xc3};//机器指令 功能：将参数+1后返回
    unsigned char * memory = static_cast<unsigned char *>(mmap(NULL,getpagesize(),PROT_READ | PROT_WRITE | PROT_EXEC,MAP_ANON | MAP_SHARED,0,0));//申请一块连续的内存空间
    for (int i = 0; i < 18; i++) {//将机器指令写入申请的内存空间
        *(memory + i) = code[i];
    }
    Incr incr = (Incr) memory;//声明函数指针 并将其指向写好了机器指令的内存空间
    int res = incr(1); //调用这个方法 实现 +1
    printf("%d\n", res);//输出 结果：2
    return 0;
}

```
输出：
2

这就是一个完整的用c++调用机器指令的例子！


**<p id="CallStub">5.2机器指令入口CallStub：</p>**

**<p id="CallStub简介">5.2.1CallStub简介：</p>**

上一小节讲到我们需要手工还设计机器指令，使得我们的Java执行效率大大提高。但这个机器指令的设计需要有一个范围，总不能用机器指令写完整个JVM，除了工作量上的难度，其实也没有必要。
<br/><br/>
因为Java代码的执行本质上是Java方法的不停地调用，整个Java虚拟机的生命周期都在做这个事情，如果Java方法调用结束了，那么Java虚拟机的任务也就完成了，会停止运行。
<br/><br/>
所以我们只需要将Java方法调用与执行的部分手工设计机器指令即可，启动加载.class文件等流程就保留c++就好了。
<br/><br/>
显然，由c++代码进入自定义机器指令，是一个很重要的过程，它意味着后面所有的代码执行都将由我们自定义的机器指令接管，意味着我们要自己决定该如何使用寄存器，如何使用栈、堆。在这之前都是由编译器决定的。
<br/><br/>
既然我们圈定了设计机器指令的范围，仅设计Java方法的调用，那么很显然我们给Java方法的调用设计一个统一的入口，用来处理寄存器、堆栈，将它们变成我们希望的样子是一个不错的选择。
<br/><br/>
在Hotspot中这个入口就是CallStub,它负责接收Java方法执行所需要的所有数据，例如参数、参数数量、被调用者等共八个参数：

| 参数名  | 含义  |
| ----  | ----  |
|    link |  连接器 |
|    result_val_address |  函数返回地址 |
|    result_type |  函数返回类型 |
|    method |  java方法在c++中所对应的对象 |
|    entry_point |  创建java方法执行环境的一段机器指令的入口 |
|    parameters |  java方法参数集合的首地址 |
|    parameters_count |  java方法参数数量） |
|    CHECK |  当前线程对象 |

为了避免对你造成误导，也是因为CallStub比较重要，所以这里把它需要的参数全都列出了，我们挑几个重要的介绍一下（当然都很重要哈哈）。
<br/><br/>
* method: 还记着我们在解析第一大章  [解析“方法”](#方法) 怎么做的吗？我们把每个Java方法都映射成了MethodInfo对象保存了起来。现在当我们想调用这个方法的时候，就需要把这个MethodInfo对象传给CallStub，另外，在CallStub之后，我们
  都是直接用机器指令编写逻辑，将不能再使用c++的方式从这个MethodInfo对象中取出想要的数据，例如：

```c++
BytecodeStream *bytecodeStream = methodInfo->getAttributeInfo()->getCode();//取出方法中的JVM指令
```

在机器指令中我们将无法通过上面这种形式处理MethodInfo对象，那怎么办呢？其实一个对象所占的内存空间也是连续的，MethodInfo对象的地址本质上就是这段连续的内存空间的首地址，那么我们只需要将这个首地址加上相应的偏移，
就能取出MethodInfo对象中的任何数据。举一个温柔的例子：

假如我们的MethodInfo声明如下：
```c++
class MethodInfo { 
public:
    int code;
    string name;
};
```

code 是int类型，内存对齐一下也就是占用8个字节，那么在我们创建一个MethodInfo对象m时，只需要把这个对象m的地址+8，就能取到name的值，不信你看：
```c++
int main() {
    MethodInfo *m = new MethodInfo;//创建对象
    m->code = 6;//code赋值为6
    m->name = "JVM"; //name赋值为JVM
    printf("%d\n", *(char *) m);
    printf("%s\n", (char *) m + 8);
    return 0;
}
输出：
6
JVM

```
那么到了机器指令中，假如m这个参数在rcx寄存器中，你想要把name放入rax寄存器中，只需要：
```c++
mov rax,[rcx+8] //机器指令 ：48 8b 41 08
```
so easy!

* entry_point:
  每个Java方法的执行都需要经过它，创建栈帧、局部变量表等就是这小子干的。

* parameters：
  每个java函数的参数都在一段连续的内存空间中，这个parameters就是这段空间的首地址。

* parameters_count：
  参数数量，CallStub会根据它来计算这个方法的执行所需要的栈空间大小，然后据此动态分配栈空间。

**<p id="CallStub实现">5.2.2CallStub实现：</p>**

**本次commit :** 138083c55447165530bf6ae003e5a158a0187c2d

先汇总一下需要做的事情再分开解释：

* 保存调用者栈基地址
* 保存需要用到的寄存器
* 分配栈空间
* 将Java方法入参保存在栈中
* 调用entry_point
* 回写返回值

* 保存调用者栈基地址：
<br/><br/>
在 [cpu是如何执行指令的](#cpu是如何执行指令的)  [cpu是如何执行方法的](#cpu是如何执行方法的) 中我们知道为了正确地恢复调用者的栈，这就需要被调用者保存调用者的栈基地址，也就是：

```
"push  rbp"
"mov  rbp,rsp"
```
这是一段通用的代码，不再过多解释。
<br/><br/>
* 保存需要用到的寄存器：
<br/><br/>
因为是我们人工设计机器指令的逻辑，为了避免我们想要使用的寄存器中的值被覆盖掉而引发不可控的结果，我们必须在使用之前，将需要使用的寄存器中的值保存在栈中，可以用mov指令，或者push指令：

```
"push rcx"
"push rdi"
"push rsi"
"push rbx"
```

* 分配栈空间：
因为后面要将java入参保存在栈内存当中，因此必须提前分配栈空间，这就要求我们需要计算出应该分配多大的内存空间，你或许会有疑问，Java参数大小不一样，如何计算呢，其实除了基本数据类型，Java所有的对象在c++中都是一个
指针，那就意味着在64位机器下，只要是Java对象，这个参数的大小就是8个字节（不考虑指针压缩），那么我们还记得CallStub的8个入参中有parameters_count这么一个参数吗，我们只需要把它左移3位（也就是乘8）
就是我们需要分配的栈内存大小，在前文中我们已经学得分配栈空间的指令：sub，那么我们的汇编指令入下：
```
"mov rcx ,[rbp+0x10]" 将rsp寄存器往上16个字节的位置的值放入rcx寄存器中
"shl rcx,3" 将rcx中的值左移3位（*8）
"sub rsp,rcx" 分配栈空间
```

关于[rbp+0x10]的解释：这个地方的值就是parameters_count，至于为什么，在 [cpu是如何执行指令的](#cpu是如何执行指令的)  [cpu是如何执行方法的](#cpu是如何执行方法的) 中我们已经知道，如果某个方法的入参
大于6个，那么前6个的参数将会通过寄存器edi、rsi、edx、rcx、r8d、r9依次进行传递，大于6的参数将会放入调用者的栈空间当中，很显然，当调用进CallStub时，这8个参数的位置如图所示：

<img src="https://github.com/ArosyW/picture/blob/master/Callstub.png" width = "450" height = "300" />


* 将Java方法入参保存在栈中：
<br/><br/>
方法的参数是属于局部变量的，因此必须复制一份放入这个方法的局部变量表中，当然，此时局部变量表还并没有创建，它会在后面的entry_point中创建，为了减少一次内存赋值，局部变量表中的入参，
将会复用CallStub中保存的java入参，而不会再创建一份。
```
    "mov rcx ,[rsp+0x10]"
    "dec rcx"
    "i:test rcx rcx" 如果rcx的值为0（无入参，则跳转到o处执行）
    "je o"
    "xor rbx,rbx"
    //循环入参start
    "shl rcx,3"
    "mov [rsp+rcx],r9"
    "shr rcx,3"
    "dec rcx"
    "inc r9"
    "jne i"
    "o:mov rsp,r9"
    //循环入数end
```
执行完这一段汇编代码之后的栈如图：

<img src="https://github.com/ArosyW/picture/blob/master/putArg.png" width = "450" height = "300" />

* 调用调用entry_point：
entry_point像CallStub一样也是在JVM启动时动态生成的一段机器指令，在方法调用时传进来它的内存首地址，用call指令就可以直接调用。
```
"mov rcx ,[rsp+0x10]"
"mov rbx,rbp"
"call [r8d]"
```
* 回写返回值：
将方法的返回值写入调用者的栈内存中。


**<p id="entry_point">5.3方法执行环境的准备entry_point：</p>**

前文说到由c++代码进入机器指令的入口:CallStub，在CallStub中会调用entry_point，同CallStub，entry_point也是通过在JVM启动时向内存中写入机器指令来实现功能的方法，或者说是一段由我们自己设计的
机器指令。

entry_point所实现的主要功能是创建局部变量表、创建栈帧、执行JVM指令。

* 创建局部变量表:
局部变量表，除了在方法内声明的变量之外，方法的入参本身就属于局部变量，而方法的入参已经在entry_point的调用者的栈中了，也就是在CallStub的栈中。那么只需要为非入参的局部变量分配栈空间即可，而且局部变量表的大小我们已经
在   [解析“方法”](#方法) 存起来了，通过CallStub的入参MethodInfo* method即可取到，然后通过多次的 push 0x0 指令实现局部变量表的创建。

> 聪明的你或许会有疑问，为什么不用sub指令直接扩栈呢？
> 因为对于Java方法的执行来说，除了要有可用的内存空间外，还需要保证这块内存空间中不会有脏数据。而在我们自己设计机器指令的时候，不停的扩栈、缩栈，难免有脏数据，在使用之前、在让这块内存作为Java的局部变量表之前，需要我们手动清理。
> 那就不如直接使用push 0x0来实现栈分配了，要知道push指令会将rsp寄存器的指向向下扩展8个字节，在分配的同时，将内存清理了。

但在真正的分配栈内存之前，让我们先来看一下此时的堆栈图：
<br/><br/>
<img src="https://github.com/ArosyW/picture/blob/master/entry_point_pre.png" width = "450" height = "300" />

由于CallStub在调用entry_point时使用的是call指令，这条指令会将CallStub下一条指令入栈，因此entry_point的栈于java入参之间被隔断了，前面我们说过entry_point想要复用CallStub的java入参而不想再复制一份，假如此时entry_point
直接扩栈，就会让局部变量表被"CallStub下一条指令"分割成两段，这样实在是太没有艺术气息了！

所以我们在真正的创建局部变量表之前，先执行 pop eax ,也就是先将"CallStub下一条指令"放入eax寄存器中存起来，在 push 0x0 进行扩栈：

<img src="https://github.com/ArosyW/picture/blob/master/entry_pointjubu.png" width = "450" height = "300" />

* 创建Java栈帧：

栈帧大小：创建Java栈帧无非就是用机器指令分配一块栈空间，但分配多大的，在Java方法执行过程中会不断地向栈帧中放入、弹出变量，似乎我们需要一个最大值，也就是这个方法的栈帧所能容纳的最大的变量的数量，而这个值编译器已经帮我们做好了，
而且在   [解析“方法”](#方法) 中给我们也已经把它存起来了，就在MethodInfo中的maxStack属性里。

在这之前我们需要先恢复"CallStub下一条指令"，将它从eax寄存器中入栈。然后再创建栈帧。

* 执行JVM指令：

在JVM启动时也会为200多条JVM指令一一生成一段机器指令，这些指令的数量是要远远小于"字节码解释器"，也就是用c++进行JVM指令解释所编译后的机器指令的数量的。在entry_point创建完局部变量和栈帧之后（当然还有很多其他细节），就可以直接调用这些机器指令了。

**<p id="执行引擎完结">6.执行引擎完结：</p>**

整个执行引擎部分可以说是Java代码能够执行的最核心的一个模块，我们前面先是用c++代码对JVM指令进行解释，成功打印了"HelloJVM"，但因为用c++解释jvm指令会编译出巨量的机器指令，从而大大降低了Java的执行效率，例如一个简单的JVM入栈指令，用c++
代码来解释将会需要用到stack结构，我们确实可以导入第三方或者我们自己实现好了的stack包，然后stack.push(arg)就能完成入栈指令的解释，但stack.push(arg)这个方法的执行却不是简简单单的一行代码，push(arg)这个方法里面会有更深的层级导致会执行大量的逻辑。

> 从内存的角度来说，用了stack包(字节码解释器)可以说是JVM的栈是基于JVM进程的堆空间实现的。

因此有了我们的"模版解释器"，通过编写机器指令，然后通过函数指针用c++调用到我们自定义的机器指令，直接将操作系统的栈作为JVM的栈，用相比于字节码解释器更少的机器指令来对JVM指令进行解释，因为JVM指令大部分都是在对JVM栈进行操作，所以模版解释器借用操作系统的栈来作为JVM的栈可以说是提升Java
执行效率的关键所在。

> 从内存的角度来说，模版解释器可以说是JVM的栈是基于操作系统的栈实现的。

当然最后很遗憾的地方是模版解释器仅仅提供了CallStub的机器码，介绍了一下entry_point，并且没有详细解析Hotspot模版解释器是如何实现的。大量机器指令的编写的工作量是巨大的，让我不得不重新思考我对模版解释器进行实现的意义与必要性。其实无论是本章节还是前面的类加载器、后面的内存部分，都只能作为你深入研究Hotspot的
入门指引，如果想要更加深入地剖析Hotspot，还需要结合更多书籍与源码。


### (三)内存池

内容与代码在都努力的憋……
需要一段时间。

### (四)GC

### (六)扩展内容

### (七)勘误

>错误commit：指含有错误的commit<br/>
>修复commit：指修复错误的commit

我尽量少写bug！

**<p id="多循环">1. 解析常量池时多循环了一次：</p>**

**错误commit :** 9a212900d255cda698780d96dafff43dc1152677
<br/><br/>
**修复commit :** a340887eaa2ef20cb609c3dd90dce7af54c8bb67
<br/><br/>
在解析"访问权限"小节顺带着修复了。
<br/><br/>
**<p id="LDC指令">2. LDC指令中因内存失效而产生的bug：</p>**

**错误commit :** 03c113485ab72a91b6cd57f4da46fe2752f1614a
<br/><br/>
**修复commit :** 4763f3e13e6142dd6dd4f9e458f454e4989398b6
<br/><br/>
LDC指令中应将常量池中的值复制一份推向栈顶。
<br/><br/>
原错误做法：未重新申请内存，导致将某个方法的局部变量的首地址指针 推向了栈顶，方法结束后内存已经被释放掉了。
<br/><br/>
修复方法：用new关键字在堆（不是java的堆）中申请一块内存专门来存储复制后的值。
