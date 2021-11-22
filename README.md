# JVM

#### 一、介绍

本项目将尝试性的一步一步实现jvm的主线内容，并对每一次代码的提交进行讲解，每一次讲解都尽可能的假设你一无所知。考虑到来此学习的大部分是Java程序员，所以本项目的C++代码将充满着Java的味道。

 * 适合人群：面向java虚拟机的初学者、想要自实现jvm的学习者。
 如果想自己动手，那么你需要有一定的C\C++基础（可以理解指针即可）。如果仅仅是学习，那么相信强大的你不需要学过C\C++也能看懂。
 
 * 参考资料：《Java虚拟机规范》
 
---
   
#### 二、目录（不定时更新……）

##### (一)[类加载模块](#类加载模块)
###### 0.[一切的开始](#一切的开始)
###### 0.5[初识字节码文件](#初识字节码文件)
###### 1.[字节码文件读取到内存](#字节码文件读取到内存)
###### 2.[解析内存中的字节码文件](#解析内存中的字节码文件)
##### (二)字节码执行器
###### 1.字节码解释器 
###### 2.模版解释器 
##### (三)内存池
###### 1.Java进程总内存
###### 2.堆区
###### 3.方法区
##### （四）GC
###### 1.标记 && 清除 && 整理
###### 2.新生代 && 老年代 
##### （五）即时编译

---
      
#### 三、猛男讲解
<br/><br/>
PS：每一章节都会进行单元测试，为了简洁，文章部分地方可能只给出方法的声明，但会给出此次提交的commit,里面包含了方法的实现。
<br/><br/>
##### 类加载模块：

---

###### 一切的开始:
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

---

###### 初识字节码文件:
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

---

###### 字节码文件读取到内存
   **本次commit :** f4f61fcfe7f461d69a58d01b2e71bb6efe4e5387 
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
单元测试一下
```c++
int main() {
    ClassRead *classRead = ClassRead::readByPath("/Users/e/Documents/github/JDK/out/production/JDK/HelloJVM.class");//换成你自己的path
    printf("%X\n", classRead->readByFourByte());
    return 0;
}
```
输出：CAFEBABE 

小总结：到这你已经成功的完成了字节码文件解析的第一步：验证这个文件是否是一个字节码文件！你真厉害，奖励自己一局”亚索“吧！

---

###### 解析内存中的字节码文件

* 解析“魔数”

   **本次commit :** 27891213b2e41614e8e19cb51c12fd9a69085ced

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
我们来做一个单元测试<br/><br/>
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

* 解析“版本号”

  **本次commit :** 20d7b7b7072bce3a83d730274a2f5078463b2a09

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
    klass->setMinorVersion(classRead->readByTwoByte());
    klass->setMajorVersion(classRead->readByTwoByte());
    switch (klass->getMajorVersion()) {
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
来测试一下：

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

* 解析“常量池”












