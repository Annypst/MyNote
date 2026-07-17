# 构造、析构

### Keywords

Constructor and Destructor,Constructor Initializer List,Access Specifiers

### 一、 构造函数 (Constructor) —— 诞生之礼

- **物理语义**：在对象诞生在内存（栈或堆）的瞬间，由编译器自动调用且仅调用一次，用于开辟空间及变量初始化。
- **核心语法**：函数名与类名完全一致，**无任何返回值类型**（连 `void` 也不能写）。可以进行函数重载。

### 二、 析构函数 (Destructor) —— 终结之美

- **物理语义**：在对象生命周期结束、即将被抹去内存的瞬间自动调用。主要用于**释放指针所指向的堆内存 (delete)、关闭打开的文件、网络套接字 (Socket) 等资源**。
- **核心语法**：名称为 **`~类名`**，**无参数、无返回值、无法被重载**。

### 三、 访问控制说明符 (Access Specifiers)

- **三大修饰符**：
  - `public`（公有）：任何外部、内部、子类均可访问。
  - `protected`（保护）：仅类内部及子类可以访问，外部禁止。
  - `private`（私有）：仅类内部成员函数可以访问，子类及外部均禁止。
- **C++ 独特规范**：
  - **区域化划分**：使用 `public:` 挂牌，直到遇到下一个修饰符，中间管辖一大片区域。
  - **`class` 与 `struct` 的唯一区别**：`class` 默认访问控制权限为 `private`；`struct` 默认访问控制权限为 `public`。

### 四、 构造函数初始化列表 (Constructor Initializer List)

- **机制**：在构造函数参数列表后，使用 **单冒号 `:`** 引出成员变量初始化序列。
- **严格顺序规则**：初始化列表中的变量初始化顺序，**只与变量在类中声明的先后顺序一致**，与其在初始化列表中的书写顺序无关。
- **黄金准则**：能用初始化列表，就绝对不要在大括号 `{}` 内部赋值。

> [!NOTE]
>
> 初始化列表中的变量初始化顺序，**只跟它们在类中声明的顺序一致**，而跟你写在初始化列表里的前后顺序无关！ 比如在类中先声明了 `name`，再声明了 `age`。即使你写成 `: age(age), name(name)`，编译器依然会先去初始化 `name`，再去初始化 `age`。

### Example Programs

1.Constructor and Destructor

```cpp
#include <iostream>
#include <string>

class Cube {
private:
    std::string name;

public:
    // 1. 有参构造函数 (Constructor)
    Cube(std::string name) {
        this->name = name;
        std::cout << "【诞生】方块 " << name << " 被创建了！" << std::endl;
    }

    // 2. 析构函数 (Destructor)
    ~Cube() {
        std::cout << "【毁灭】方块 " << this->name << " 被销毁了！~" << std::endl;
    }
};

int main() {
    std::cout << "--- main 函数开始 ---" << std::endl;

    // 场景 A：创建在【栈区】的局部对象
    {
        std::cout << "\n进入局部作用域 {}" << std::endl;
        Cube stackCube("栈区小一"); // 栈区对象创建
        std::cout << "离开局部作用域 {} 之前" << std::endl;
    } // 出了大括号，栈区小一的生命周期到头了

    std::cout << "\n------------------------------------" << std::endl;

    // 场景 B：创建在【堆区】的手动对象
    std::cout << "\n准备在堆区 new 一个对象" << std::endl;
    Cube* heapCube = new Cube("堆区老二"); // 堆区对象创建

    std::cout << "准备手动 delete 堆区对象" << std::endl;
    // 如果不写下面这行 delete，heapCube 就永远不会被销毁（发生内存泄漏）
    delete heapCube; 

    std::cout << "\n--- main 函数结束 ---" << std::endl;
    return 0;
}
```

2.Constructor Initializer List

```cpp
#include <iostream>
#include <string>

class Scholar {
private:
    const std::string university; // 只读的学校名称（const 成员）
    int id;

public:
    // 正确写法：使用初始化列表。const 变量在诞生的一瞬间被初始化！
    Scholar(std::string uni, int id) : university(uni), id(id) {
        // university = uni; // ❌ 错！如果把这行解开，编译器会暴怒，因为 const 变量进入大括号后就无法再赋值了
        std::cout << "学者初始化成功！学校: " << this->university << ", ID: " << this->id << std::endl;
    }
};

int main() {
    // 在栈上创建一个 Scholar 对象
    Scholar s("Taiyuan University of Technology", 2026); //

    return 0;
}
```

