#  拷贝构造与深浅拷贝

### Keywords

Copy Constructor , Deep/Shallow Copy

### 一、 拷贝构造函数 (Copy Constructor)

- **物理语义**：用一个已存在的对象，去克隆初始化另一个同类的新对象。
- **核心语法**：参数必须是**当前类对象的常量引用 (Constant Reference)**，即 `const ClassName& other`。
- **为什么必须是引用？**：若使用值传递 (Pass-by-value)，传参时编译器又会调用拷贝构造，从而陷入“传参-拷贝-再传参-再拷贝”的无限递归，最终导致**栈溢出 (Stack Overflow)** 崩溃。

### 二、 浅拷贝 (Shallow Copy)

- **物理行为**：逐个字节复制对象的成员变量，在底层也叫**等号赋值 / 逐成员拷贝 (Memberwise Copy)**。
- **物理灾难 (Double Free)**：若类中含有指针成员并指向堆区 (Heap)，浅拷贝只会复制指针的十六进制地址。导致两个对象的指针指向同一块物理内存。当两对象析构时，会分别尝试 `delete` 该地址，从而触发**双重释放 (Double Free / Memory Corruption)** 崩溃。

### 三、 深拷贝 (Deep Copy)

- **物理行为**：不直接复制指针地址，而是**在堆区重新申请一块同样大小的物理内存**，并将源内存中的值复制到这块新内存中。
- **物理效果**：各个对象拥有完全独立的堆内存地址，生命周期结束时各自释放，平稳析构。

### 四、 堆内存管理器的“隐形记账”成本

- **现象**：在堆区分配一个 `int`（4 字节），但连续分配两次的内存地址却相差了 **`0x20`（32 字节 / Bytes）**。

- **底层原理解密**：

  1. **控制块 (Chunk Header / Metadata)**：64位系统下，堆内存分配器 (Heap Allocator) 会在返回地址前默默开辟 **`16` 字节**，用于记录内存大小等记账信息（所以 `delete` 无需传大小）。
  2. **分配粒度对齐 (Alignment Boundary)**：操作系统为了检索效率，要求每次分配的堆块大小必须是 **`16` 字节的整数倍**。

  - **计算公式**：
    $$
     4\text{字节 (int)} + 16\text{字节 (Header)} = 20\text{字节} 
    $$
    

    为了满足 16 字节的倍数，向上对齐到 **`32` 字节**。

### Hints

在学术和高级系统架构中，有一个著名的设计模式叫 **原型模式（Prototype Pattern）**。 它的核心思想是：**“通过克隆已有对象来创建新对象，而不是通过构造函数。”**

- **痛点**：有时候，通过 `new` 去构造一个新对象的开销是极其昂贵的。比如，初始化一个对象需要去读取硬盘里的 3D 模型配置文件、连接数据库读取用户信息、或者进行极其复杂的物理算力运算（需要耗时 2 秒）。
- **解决**：我们只需要在程序启动时用 `new` 慢吞吞地初始化好一个“母本”。后续需要新对象时，直接通过拷贝构造（深拷贝）在内存中瞬间克隆出来，避开了耗时的硬盘 I/O 和数据库读取。

### Example Programs

1. Double Free 双重释放崩溃

```cpp
#include <iostream>

class Person {
public:
    int age;
    int* height; // 指针成员：指向堆区存放的身高数据

    // 1. 有参构造函数
    Person(int age, int h) {
        this->age = age;
        // 在堆区开辟一块内存来存身高
        this->height = new int(h);
        std::cout << "【构造】Person 创建成功！" << std::endl;
    }

    // 2. 析构函数（释放堆区开辟的内存）
    ~Person() {
        if (this->height != nullptr) {
            std::cout << "【析构】准备 delete height 指针指向的地址: " << this->height << std::endl;
            delete this->height;
            this->height = nullptr; // 擦除指针，防止野指针
        }
    }

    // 💡 场景 A：不写拷贝构造函数，直接使用编译器默认提供的【浅拷贝】
    
    /* 
    // 💡 场景 B：手动写一个【深拷贝】构造函数
    Person(const Person& other) {
        this->age = other.age;
        // 【核心操作】：重新在堆区申请一块干净的内存，复制值，而不是直接复制指针地址！
        this->height = new int(*(other.height)); 
        std::cout << "【深拷贝构造】重新开辟堆区空间成功！" << std::endl;
    }
    */
};

int main() {
    std::cout << "--- main 开始 ---" << std::endl;

    {
        Person p1(20, 180);
        
        std::cout << "\n开始用 p1 克隆 p2..." << std::endl;
        Person p2 = p1; // 触发拷贝构造！

        std::cout << "p1 的 height 内存地址: " << p1.height << std::endl;
        std::cout << "p2 的 height 内存地址: " << p2.height << std::endl;
        std::cout << "克隆结束，即将出了 {} 作用域销毁两个对象...\n" << std::endl;
    }

    std::cout << "--- main 正常结束 ---" << std::endl;
    return 0;
}
```