#  类与对象在内存中的本质

### Keywords

Class,Object,Memory Alignment,Memory Padding,Addressing 

### 一、 类 (Class) 与 对象 (Object)

- **类 (Class)**：仅仅是**模具/设计图纸**，属于编译期概念，在物理内存中**不占用任何空间**。
- **对象 (Object / Instance)**：根据类图纸在内存中真正浇筑出来的**物理实体**，在运行期**占用实际的内存空间**。

### 二、 空类的大小 (Size of Empty Class) —— 1 字节的尊严

- **现象**：没有任何成员变量和成员函数的空类，其对象大小为 **`1` 字节**。
- **底层原因**：C++ 编译器强制要求任何独一无二的对象在内存中必须有**唯一的内存地址 (Unique Address)**。如果为 `0` 字节，多个空类对象的物理地址将会重叠。编译器默默塞入的 `1` 字节仅作为**占位符 (Placeholder)**。

### 三、 成员变量与成员函数的物理分离机制

- **成员变量 (Member Variable)**：存放在**每个对象专属的内存空间内**。对象占用的内存大小，本质上仅由其非静态成员变量决定。
- **成员函数 (Member Function)**：存放在公共的**代码区 (Code Area)**。全宇宙只有一份拷贝，所有对象共享这一段函数代码。

### 四、 内存对齐 (Memory Alignment) 与 内存填充 (Padding)

- **现象**：一个包含 `char`（1 字节）和 `int`（4 字节）的对象，其实际大小并非 `5` 字节，而是 **`8` 字节**。
- **对齐原因**：虽然内存是**按字节寻址 (Byte-addressable)** 的，但 CPU 通过**数据总线 (Data Bus)** 和内存控制器传输数据时，是按照**字长 (Word-size)** 边界（如 64 位系统下为 8 字节块）进行**对齐数据传输 (Aligned Data Transfer)** 的。
- **内存填充 (Padding)**：为了避免 CPU 跨越边界读取单个变量进行两次内存访问，编译器会在较小成员后自动填充废字节。这是典型的**空间换时间 (Space-Time Tradeoff)** 优化策略。

### 五、 `this` 指针的底层魔法 (`this` pointer)

- **痛点**：既然公共代码区只有一份成员函数，函数如何精准分辨是谁在调用它？
- **底层行为**：C++ 编译器在编译非静态成员函数时，会自动在参数列表第一位植入一个**隐式参数 (Implicit Parameter)**，即 **`Type* const this` 指针**。
- **调用对账**：当执行 `obj.func()` 时，编译器在底层自动翻译为 `func(&obj)`。函数内部所有对成员变量的读写，在底层都被自动重写为通过 `this->` 指针进行访问。
- 总结一下，`this` 指针的三个主要显式用法：
  1. **防重名**：`this->name = name;`（区分同名形参与成员变量）。
  2. **链式调用**：`return *this;`（返回当前对象本身的引用）。
  3. **自我外传**：`func(*this);` 或 `func(this);`（把自己扔给外部函数）。



### Graphs

计算机的中央处理器（CPU）在从内存条（RAM）读取数据时，并不是一个字节一个字节去读取的。为了提高效率，CPU 会以内存块（Granularity / Chunk）为单位进行读取，在 32 位系统下通常是 `4` 字节，在 64 位系统下通常是 `8` 字节。

假设我们不进行内存对齐，把 `c`（1 字节）和 `i`（4 字节）紧挨着紧凑存放：

```
不合法的紧凑存放（假设 CPU 每次读取 4 字节）：
[ 内存块 1 ] ─────────────────────────► [ 内存块 2 ]
┌──────────┬──────────┬──────────┬──────────┐ ┌──────────┬──────────┬──────────┬──────────┐
│  char c  │  int i   │  int i   │  int i   │ │  int i   │  (空闲)  │  (空闲)  │  (空闲)  │
└──────────┴──────────┴──────────┴──────────┘ └──────────┴──────────┴──────────┴──────────┘
 字节 0      字节 1     字节 2     字节 3      字节 4     字节 5     字节 6     字节 7
```

如果我们要读取整个 `int i` 的值：

1. CPU 第一次读取【内存块 1】，拿到了 `i` 的前 3 个字节。
2. CPU 第二次读取【内存块 2】，拿到了 `i` 的最后 1 个字节。
3. CPU 在内部把这两部分数据拼凑在一起。

**为了读一个 `int`，CPU 居然要读两次内存并进行拼凑，这太慢了！**

为了让 CPU 读得飞快，编译器会在 `char c` 后面，默默地塞入 `3` 个不使用的废字节。这在英文中叫做 **Padding（填充）**：

```
进行内存对齐（Memory Alignment）后的布局：
[ 内存块 1 (4字节) ]                            [ 内存块 2 (4字节) ]
┌──────────┬──────────┬──────────┬──────────┐ ┌──────────┬──────────┬──────────┬──────────┐
│  char c  │ [Padding]│ [Padding]│ [Padding]│ │                 int i                 │
└──────────┴──────────┴──────────┴──────────┘ └──────────┴──────────┴──────────┴──────────┘
 字节 0      字节 1     字节 2     字节 3      字节 4     字节 5     字节 6     字节 7
```

此时：

- `char c` 占了第 0 字节。
- 第 1, 2, 3 字节被 **Padding（填充）** 填满。
- `int i` 完美地从第 4 字节（也就是第二个 4 字节块的开头）开始存放。
- CPU 只需要一次读取【内存块 2】，就能完整、干净地拿到 `int i`！

这种用空间换时间（Space-Time Tradeoff）**的做法，就是**内存对齐（Memory Alignment）的底层逻辑。

### Example Programs

1.Empty Class

```cpp
#include <iostream>

// 定义一个空类
class EmptyClass {
    // 里面没有任何成员变量，也没有任何成员函数
};

int main() {
    // 实例化一个空类的对象
    EmptyClass obj;

    // sizeof 可以用来查看一个对象在内存中占用的字节数
    std::cout << "空类对象 obj 占用的字节数: " << sizeof(obj) << " 字节" << std::endl;

    return 0;
}
```

2.The Space of the variable and function

```cpp 
#include <iostream>

class TestClass {
public:
    int a; // 占用 4 字节

    // 成员函数
    void printHello() {
        std::cout << "Hello C++!" << std::endl;
    }
};

int main() {
    TestClass obj;
    
    std::cout << "带有 1 个 int 变量和 1 个函数的对象大小: " 
              << sizeof(obj) << " 字节" << std::endl;

    return 0;
}
```

3. this.pointer

```cpp
#include <iostream>

class Person {
public:
    int age;

    // 成员函数
    void showMyAddress() {
        // 在类内部，可以直接通过关键字 this 拿到当前对象的地址
        std::cout << "内部 [this 指针] 的值是:   " << this << std::endl;
    }
};

int main() {
    Person zhou;
    Person wu;

    // 1. 打印 zhou 在外部的真实物理地址
    std::cout << "外部 [zhou 对象] 的真实地址: " << &zhou << std::endl;
    // 2. 调用成员函数，看看内部拿到的 this 是不是 zhou 的地址
    zhou.showMyAddress();

    std::cout << "------------------------------------------" << std::endl;

    // 3. 打印 wu 在外部的真实物理地址
    std::cout << "外部 [wu 对象] 的真实地址:   " << &wu << std::endl;
    // 4. 调用成员函数，看看内部拿到的 this 是不是 wu 的地址
    wu.showMyAddress();

    return 0;
}
```

4.this.chaining

```cpp
#include <iostream>
#include <string>

class Player {
private:
    std::string name;
    int score = 0;

public:
    // 每一个设置函数，最后都返回当前对象本身的引用：Player&
    Player& setName(std::string name) {
        this->name = name;
        return *this; // 【核心点】：*this 拿到的就是当前对象本身的引用！
    }

    Player& addScore(int points) {
        this->score += points;
        return *this; // 返回自身，允许后面继续点点点调方法
    }

    void show() {
        std::cout << "玩家: " << name << ", 当前积分: " << score << std::endl;
    }
};

int main() {
    Player p;
    
    // 链式调用（Chaining）：爽快的连击写代码
    p.setName("周汇旋").addScore(100).addScore(50).show(); //

    return 0;
}
```

5.this.callbacks

```cpp
class Printer; // 前置声明（Forward Declaration）

class Document {
public:
    std::string content = "这是太原理工大学的实验报告内容..."; //

    // 打印文档的方法
    void printMe(Printer& printer); 
};

class Printer {
public:
    // 外部函数接收一个 Document 的引用
    void doPrint(Document& doc) {
        std::cout << "打印机正在打印: " << doc.content << std::endl;
    }
};

// 实现 Document 的方法
void Document::printMe(Printer& printer) {
    // 把自己（*this）作为参数传给打印机
    printer.doPrint(*this); 
}
```

