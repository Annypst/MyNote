# Java之反射

## 引言

什么是反射？

反射允许对封装类的字段，方法和构造函数的信息进行编程访问。

说人话？

反射允许对成员变量，成员方法和构造方法的信息进行编程访问。就是我们可以在外部取出一个类的东西来用，idea的自动补全就是通过这个来实现的，类似于一个箱子里有工具，我们可以从外部取出里面的工具，并且获取他们所有的信息。

照镜子->-看到镜像->**reflection**->翻译成反射

<img src="assets/image-20251208192203498.png" alt="image-20251208192203498" style="zoom:50%;" />



～本文为黑马程序员Java配套笔记



即 获取  解剖

但是，这是从class字节码文件中获取的。

## 获取class对象

```java
//获取class对象的3种方法
Class.forName("FULL class_name");//源代码阶段
类名.class;//加载到内存阶段
对象.getClass();//创建对象阶段（运行阶段） 
```

首先我们需要一个类，我定义了一个标准的Student类：

```java
package com.annypst.fanshe;

public class Student {
    String name;
    int age;
    public Student(String name, int age) {
        this.name = name;
        this.age = age;
    }
    public Student() {

    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }

}

```

然后依次用三种方法对这个类反射：

```java
package com.annypst.fanshe;

public class Fanshe {
    public static void main(String[] args) throws ClassNotFoundException {
        //first 最常用
        Class cla=Class.forName("com.annypst.fanshe.Student");
        //second 作为参数传递 method(Student.class)
        Class cla2=Student.class;
        //third 我们已经有了对象才能使用
        Student s1=new Student();
        Class cla3=s1.getClass();
        System.out.println(cla);
        System.out.println(cla2);
        System.out.println(cla3);
        System.out.println(cla==cla2);
        System.out.println(cla2==cla3);

    }
}

```

结果：

`class com.annypst.fanshe.Student
class com.annypst.fanshe.Student
class com.annypst.fanshe.Student
true
true`

可见这些是等价的

## 反射获取构造方法

构造方法是Constructor这个类的对象。

成员变量（字段）是Field这个类的对象。

成员方法是Method这个类的对象。

为了方便演示，我给上面的student类又增加了私有的构造方法。

```java
package com.annypst.fanshe;

public class Student {
    String name;
    int age;

    public Student(String name, int age) {
        this.name = name;
        this.age = age;
    }

    public Student() {

    }

    private Student(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

}

```

然后我们可以通过这些方法获取不同的构造方法：

```java
package com.annypst.fanshe;

import java.lang.reflect.Constructor;

public class Fanshe {
    public static void main(String[] args) throws ClassNotFoundException, NoSuchMethodException {
        Class cla = Class.forName("com.annypst.fanshe.Student");
        //获取所有公共构造方法
        Constructor[] cons = cla.getConstructors();
        //遍历
        System.out.println("所有公共的构造方法：");
        for (Constructor c : cons) {
            System.out.println(c);
        }
        //获取所有构造方法
        Constructor[] dcs = cla.getDeclaredConstructors();
        System.out.println("所有的构造方法：");
        for (Constructor c : dcs) {
            System.out.println(c);
        }
        //获取单个公共的构造方法
        System.out.println("获取单个公共的构造方法");
        Constructor scon1=cla.getConstructor();
        System.out.println("单个公共的无参数构造方法"+scon1);
        Constructor scon2=cla.getConstructor(String.class,int.class);
        System.out.println("单个公共的有参数构造方法"+scon2);

    }
}

```

结果：

`所有公共的构造方法：
public com.annypst.fanshe.Student(java.lang.String,int)
public com.annypst.fanshe.Student()
所有的构造方法：
public com.annypst.fanshe.Student(java.lang.String,int)
private com.annypst.fanshe.Student(java.lang.String)
public com.annypst.fanshe.Student()
获取单个公共的构造方法
单个公共的无参数构造方法public com.annypst.fanshe.Student()
单个公共的有参数构造方法public com.annypst.fanshe.Student(java.lang.String,int)`

可见，如果只是想要获取公共public的构造方法，就用getConstructors()，如果不加s，就是单个的。如果想要获取所有的构造方法，就加一个Declared。想要指定到底获取哪个单个的方法，就getConstructor(String.class,int.class)，在参数中指定你要获取的那个构造方法形参列表的参数+.class，这样就可以了。同样，后面的也一样，加上Declared，就可以获取所有的东西。



获取权限，可使用getModifiers()方法获取当前构造方法的权限，返回整数，1就是public。其他的也可查阅文档得知。

```java
 //获取权限
        System.out.println(scon1.getModifiers());
        System.out.println(scon2.getModifiers());
```

结果：

`1
1`

同样，也可以获取参数个数和类型：

```
 System.out.println(scon2.getParameterCount());
        Parameter[] parameter = scon2.getParameters();
        for(Parameter p:parameter){
            System.out.println(p);
            
```

结果：

`2
java.lang.String arg0
int arg1`

> [!NOTE]
>
> tip：使用setAccessible (true);可临时取消访问权限，这叫做**暴力反射**，通过暴力反射，就可以使用私有的构造方法创建对象了。

## 反射获取成员变量

和上面一样，只不过这次不是获取Constructor，而是Field、Fields。获取单个、多个与上面一样。

```
//获取所有成员变量
        Field[] declaredFields = cla.getDeclaredFields();
        System.out.println("所有的成员变量：");
        for(Field f:declaredFields){
            System.out.println(f);
        }
```

结果：

`所有的成员变量：
java.lang.String com.annypst.fanshe.Student.name
int com.annypst.fanshe.Student.age`

也就是String name 和int age。

如果想获取指定的，如下：

```
 Field name = cla.getDeclaredField("name");
 System.out.println("单个的成员变量"+name);
```

`单个的成员变量java.lang.String com.annypst.fanshe.Student.name`

同样也可以获取权限修饰符之类的，方法同上。



> [!NOTE]
>
> 使用.get()方法可获取当前对象的值，前提是当前对象已经赋值。
>
> 使用.set()方法可修改当前对象的值，如把上面name修改，可使用 对象名.set(name,"修改后的内容")

## 反射获取成员方法

这次变成了Method、Methods。

```
 Field name = cla.getDeclaredField("name");
        System.out.println("单个的成员变量"+name);

        Method[] methods = cla.getMethods();
        System.out.println("所有的成员方法，包括父类：");
        for(Method m:methods){
            System.out.println(m);
        }
```

结果：

`所有的成员方法，包括父类：
public java.lang.String com.annypst.fanshe.Student.getName()
public void com.annypst.fanshe.Student.setName(java.lang.String)
public int com.annypst.fanshe.Student.getAge()
public void com.annypst.fanshe.Student.setAge(int)
public boolean java.lang.Object.equals(java.lang.Object)
public java.lang.String java.lang.Object.toString()
public native int java.lang.Object.hashCode()
public final native java.lang.Class java.lang.Object.getClass()
public final native void java.lang.Object.notify()
public final native void java.lang.Object.notifyAll()
public final void java.lang.Object.wait(long) throws java.lang.InterruptedException
public final void java.lang.Object.wait(long,int) throws java.lang.InterruptedException
public final void java.lang.Object.wait() throws java.lang.InterruptedException`

可见，除了我自己定义的方法，还有很多我没有定义的方法，这些方法是继承自Object父类的。

如果我换成getDeclaredMethods(),此时就只是子类的了。





Object invoke （object obj, Object... args）： 运行方法

参数一：用obj对象调用该方法

参数二：调用方法的传递的参数（如果没有就不写）

返回值：方法的返回值（如果没有就不写）

我现在在学生类中加一个吃东西的eat方法：

```
public void eat(String food,int count) {
        System.out.println("吃了"+count+"个"+food);
    }
```



然后在反射里这么写：

```
 Student s1=new Student();
        Method eat1=cla.getDeclaredMethod("eat",String.class,int.class);
        eat1.invoke(s1,"汉堡包",4);

```

结果：

`吃了4个汉堡包`

可见，成功运行。但如果是私有方法，就得setAccessible (true);

如果有返回值，那么就得接受了。

## 小练习

1.对于任意一个对象，都可以把对象所有的字段名和值，保存到文件中去

2.反射可以跟配置文件结合的方式，动态的创建对象，并调用方法



End.
