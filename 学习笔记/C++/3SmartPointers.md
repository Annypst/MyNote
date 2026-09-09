# 智能指针通关笔记

### 一、 为什么需要智能指针？

- **传统指针的灾难**：在堆区（Heap）使用 `new` 申请内存后，如果忘记写 `delete`，会导致**内存泄漏**；如果重复 `delete`，会导致**程序崩溃**。
- **智能指针的救赎**：通过将堆内存托付给一个栈上的“智能指针对象”，利用栈对象“出大括号（生命周期结束）时会自动调用析构函数”的物理特性，实现**堆内存自动安全回收**，做到 **0 手动 delete**。

### 二、 独占式智能指针 `std::unique_ptr`

- **核心特性**：
  - **独占性**：一块堆内存只能被一个 `unique_ptr` 拥有。
  - **禁止拷贝**：不能进行赋值或复制，底层通过禁用拷贝构造函数（deleted function）来实现语法级锁死。
- **最安全创建方式**：`std::unique_ptr<Type> p = std::make_unique<Type>(value);`

### 三、 共享式智能指针 `std::shared_ptr`

- **核心特性**：
  - **共享性**：多把“钥匙”（指针）可以同时管理同一块堆内存。
  - **引用计数**：底层通过一个公共的、在堆区开辟的“控制块（Control Block）”来共同维护一个引用计数器。
  - **生死法则**：
    - 当有新的 `shared_ptr` 拷贝指向该内存，计数器 `+1`。
    - 当有 `shared_ptr` 死亡（出作用域），计数器 `-1`。
    - **只有当计数器归 0 时**，才会真正去执行 `delete` 释放堆区数据。
- **最安全创建方式**：`std::shared_ptr<Type> p = std::make_shared<Type>(value);`

### Graphs

- 所有的 `p1`、`p2`、`p3` 内部，其实都有一个暗哨，**共同指向这同一个控制块**。

```
                     ┌───────────────┐
 p1 ───(指向)─────────►│ 堆区控制块    │
                      │ 引用计数:  2  │
 p2 ───(指向)─────────►└──────┬────────┘
                              │ (共享同一个计数)
                              ▼
                        ┌───────────┐
                        │ 堆区数据  │
                        │   (99)    │
                        └───────────┘
```

所以，无论你打印 `p1.use_count()` 还是 `p2.use_count()`，它们实际上都是去**同一个控制块里读取同一个计数器数字**。这就是为什么它们的人数永远绝对同步！

### Example Programs

1.std::unique_ptr 

```cpp
#include <iostream>
#include <memory> // 必须引入这个头文件才能用智能指针

int main() {
    std::cout << "--- 程序开始 ---" << std::endl;

    // 用大括号 {} 制造一个局部的作用域（栈区）
    {
        // 声明一个独占式智能指针 p，指向堆区的一个 int(100)
        // std::make_unique 是现代 C++ 最推荐的、安全的创建方法
        std::unique_ptr<int> p = std::make_unique<int>(100);

        std::cout << "p 指向的值: " << *p << std::endl;
        std::cout << "p 的内存地址: " << p.get() << std::endl; // get() 可以拿到原始指针

        // 尝试拷贝 p（把下面这行代码的注释解开，编译一下试试看）
        // std::unique_ptr<int> p2 = p; // 报错！unique_ptr 禁止拷贝，保证独占

    } // 出了这个大括号，智能指针 p 在栈区死亡！它指向的堆区 100 会被自动 delete 释放！

    std::cout << "--- 程序出了大括号 ---" << std::endl;
    std::cout << "此时堆区内存已经被安全自动释放，完全不用写 delete！" << std::endl;

    return 0;
}
```

2.shared_ptr

```cpp
#include <iostream>
#include <memory>

int main() {
    std::cout << "--- 开始创建 shared_ptr ---" << std::endl;

    // 创建一个共享指针 p1，指向堆区的 99
    std::shared_ptr<int> p1 = std::make_shared<int>(9339);
    
    // use_count() 可以查看当前有几个指针共享这块堆内存
    std::cout << "当前共享人数: " << p1.use_count() << "， 值: " << *p1 << std::endl;

    {
        // 允许拷贝！p2 也指向同一个地方
        std::shared_ptr<int> p2 = p1; 
        std::cout << "\n--- 拷贝给 p2 后 ---" << std::endl;
        std::cout << "p1 的共享人数: " << p1.use_count() << std::endl;
        std::cout << "p2 的共享人数: " << p2.use_count() << std::endl;
        std::cout << "p2 value is: "<< *p2 << std::endl;
    } // 出了这个大括号，p2 死亡！

    std::cout << "\n--- p2 死亡后 ---" << std::endl;
    std::cout << "here we create a new copyed pointer by p1" << std::endl;
    std::shared_ptr<int> pc = p1;
    std::cout << pc.use_count() << std::endl; //the result is 2,which means that pc == p1 ,they shared both 1 shared_ptr.
    std::cout << "p1 的共享人数: " << p1.use_count() << std::endl;

    return 0;
} // p1 死亡，共享人数归 0，堆区 99 被自动 delete
```

