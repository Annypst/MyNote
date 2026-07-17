# 多态与虚函数表

### Keywords

Polymorphism,vtbl（vtable / Virtual Method Table）,Dynamic Binding,Static Binding,Virtual Pointer,Base Class,Derived Class,Virtual Destructor

### 一、 静态绑定与动态绑定

- **静态绑定（默认）**：C++ 为了极致的高性能，默认在编译期只根据“指针的声明类型”来写死调用的函数地址。
- **动态绑定**：通过给基类函数添加 `virtual` 关键字，将函数调用的决议推迟到“运行期”，根据指针实际指向的对象类型来动态调用。

### 二、 虚函数指针（vptr）与虚函数表（vtable）

- **vtable（虚表）**：类特有的一张只读表，里面按顺序存放着该类所有虚函数的真实物理地址。
- **vptr（虚指针）**：含有虚函数的类对象头部会隐式多出 **`8` 字节（64位系统下）** 的指针，指向该对象所属类的虚表。
- **物理开销**：多态会使类对象的体积膨胀（需考虑内存对齐），且每次调用虚函数需要经历三次物理跳转（指针 -> 对象 -> vptr -> vtable -> 真实代码区），有一定的性能折损。

### 三、 继承与重写规范

- **语法**：`class Derived : public Base { ... };` 冒号加继承方式。
- **重写保障**：子类重写父类虚函数时，建议在尾部显式加上 `override`，让编译器帮你严格校验函数签名。

### 四、 虚析构函数

- **铁律**：**只要一个类会被用作基类，并且内部含有虚函数，它的析构函数就必须声明为 `virtual`！**就是必须给父类的析构加虚拟关键字。
- **后果**：若不声明为虚析构，通过父类指针 `delete` 子类对象时，子类的析构函数将不会被触发，造成不可逆的**内存泄漏**与资源泄露。

### Hints

一旦加了 `virtual`，编译器的态度就会发生 180° 的大转变。它再也不敢把函数地址写死了，而是采用一种巧妙的物理机制来**动态寻找代码**。这就是著名的：

- **虚函数表（vtable / Virtual Method Table）**
- **虚函数表指针（vptr / Virtual Pointer）**

当一个类里面含有虚函数时，编译器会在底层默默地做两件事：

1. **为每个类生成一张虚函数表（vtable）**：

   - 这张表是一个**函数指针数组**，里面按顺序存放着这个类所有虚函数的真实物理地址。
   - 如果子类重写（Override）了父类的虚函数，子类的虚函数表里就会把父类的函数地址替换为**子类自己的函数地址**。

2. **在对象的最前面塞入一个隐形的虚表指针（vptr）**：

   - 当对象被 `new` 出来时，编译器会在这个对象的内存最前端（通常是偏离起始地址 `0` 的位置）默默塞入一个指针 `vptr`。

   - 这个 `vptr` 存的就是**该对象所属类的虚函数表（vtable）的起始地址**。

   - 当加上 `virtual` 后，你再调用 `myGPU->runTask();` 时，CPU 在底层并不是直接跳转去执行代码，而是像侦探一样，要经历**三次物理跳转**：

     ```
     [ myGPU 指针 ]
           │
           ▼ (第一步：顺着 myGPU 指针，找到堆区里的子类对象 NvidiaH100)
     ┌──────────────────────────────────────────┐
     │   NvidiaH100 对象的物理内存空间          │
     ├──────────────────────────────────────────┤
     │ vptr (隐形虚表指针) ──────────────────┐  │
     ├──────────────────────────────────────│───┤
     │ age, power... (其他普通的成员变量)   │  │  │
     └──────────────────────────────────────│───┘
                                            │
           ┌────────────────────────────────┘
           ▼ (第二步：顺着 vptr 指针，找到只读数据区里的 NvidiaH100 虚函数表)
     ┌──────────────────────────────────────────┐
     │ NvidiaH100 的虚函数表 (vtable)           │
     ├──────────────────────────────────────────┤
     │ [0] &NvidiaH100::runTask ───────────┐    │  ◄── 这里的父类地址已经被子类重写了！
     │ [1] &NvidiaH100::otherVirtualFunc   │    │
     └─────────────────────────────────────│────┘
                                           │
           ┌───────────────────────────────┘
           ▼ (第三步：顺着表里登记的实际函数地址，终于找到并执行子类的代码)
     ┌──────────────────────────────────────────┐
     │ 代码区 (Code Area)                       │
     ├──────────────────────────────────────────┤
     │ 执行: std::cout << "【H100 旗舰】..."    │
     └──────────────────────────────────────────┘
     ```

### Example Programs

1. No Virtual

```cpp
#include <iostream>

// 1. 基础 GPU 算力卡类
class GPU {
public:
    // 普通成员函数（未加 virtual）
    void runTask() {
        std::cout << "【基础卡】正在运行通用的算力任务..." << std::endl;
    }
};

// 2. 派生类：英伟达 H100 显卡
class NvidiaH100 : public GPU {
public:
    void runTask() {
        std::cout << "【H100 旗舰】正在全速进行大模型分布式训练！🚀" << std::endl;
    }
};

int main() {
    std::cout << "--- 测试静态绑定（未加 virtual） ---" << std::endl;

    // 用父类指针指向子类对象（在 Java 里极其高频的操作）
    GPU* myGPU = new NvidiaH100();

    // 运行任务：我们期望它运行 H100 的专属任务
    myGPU->runTask(); 

    delete myGPU;
    return 0;
}
```

2.With Virtual

```cpp
#include <iostream>

// 1. 没有任何虚函数的类
class NormalGPU {
    int id; // 4 字节
};

// 2. 含有虚函数的类
class VirtualGPU {
    int id; // 4 字节
public:
    virtual void runTask() {
        std::cout << "【基础卡】运行任务" << std::endl;
    }
};

class NvidiaH100 : public VirtualGPU {
public:
    void runTask() override { // override 关键字用于让编译器帮你检查是否成功重写
        std::cout << "【H100 旗舰】正在全速进行大模型分布式训练！🚀" << std::endl;
    }
};

int main() {
    std::cout << "--- 1. 测试动态绑定结果 ---" << std::endl;
    VirtualGPU* myGPU = new NvidiaH100();
    myGPU->runTask(); // 期待动态绑定，打印 H100
    delete myGPU;

    std::cout << "\n--- 2. 探寻隐形虚表指针 vptr 的物理存在 ---" << std::endl;
    
    // 正常类的大小应该是多少？ (只有 1 个 int 变量)
    std::cout << "NormalGPU 类对象的大小:  " << sizeof(NormalGPU) << " 字节" << std::endl;

    // 虚函数类的大小是多少？ (1 个 int 变量 + 1 个隐形的 vptr 指针)
    // 在 64 位系统下，一个指针的大小是 8 字节。 4字节(int) + 8字节(vptr) 经内存对齐后应为 12->16 字节
    std::cout << "VirtualGPU 类对象的大小: " << sizeof(VirtualGPU) << " 字节" << std::endl;

    return 0;
}
```

3.Virtual Destructor

```cpp
#include <iostream>

class BaseGPU {
public:
    BaseGPU() { std::cout << "【基类】构造" << std::endl; }
    
    // 💡 场景 A：普通析构函数（没有加 virtual）
    ~BaseGPU() { std::cout << "【基类】析构销毁" << std::endl; }
};

class SuperH100 : public BaseGPU {
private:
    int* gpuMemory; // 子类独有的指针，指向堆区的算力缓冲区
public:
    SuperH100() {
        std::cout << "【子类】构造：在堆区申请 1024 字节缓冲区" << std::endl;
        gpuMemory = new int[256]; // 申请堆内存
    }
    
    ~SuperH100() {
        std::cout << "【子类】析构：释放 1024 字节缓冲区！" << std::endl;
        delete[] gpuMemory; // 释放堆内存
    }
};

int main() {
    std::cout << "--- 准备用父类指针管理子类对象 ---" << std::endl;
    BaseGPU* gpu = new SuperH100();

    std::cout << "\n--- 准备使用父类指针 delete 对象 ---" << std::endl;
    delete gpu; // 极其高频的销毁行为

    return 0;
}
```

> [!NOTE]
>
> 给父类加了虚拟关键字，现在，父类指针变成动态绑定了。我现在要销毁子类缓冲区，好，我给gpu指针执行delete，那么，现在会顺着这个指针寻找到子类对象，发现里面vptr，好的，继续，我用vptr找到虚函数表，发现里面子类的析构，那么，我就执行这个子类析构了，那么成功释放。
>
> **寻路成功**：`delete gpu` 确实顺着 `vptr` 找到了子类的析构函数并成功执行，子类堆区的 `gpuMemory`（1024 字节）被干干净净地释放掉了。
>
> **最后一击（物理链式调用）**：当子类的析构函数 `~SuperH100()` 彻底执行完毕的一瞬间，子类对象并不会就此打住。因为子类在物理内存里是包裹着父类的，所以**子类析构会隐式、自动地向上调用父类的析构函数 `~BaseGPU()`**，把父类占用的那部分物理内存也彻底销毁。

