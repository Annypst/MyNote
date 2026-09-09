# 函数模板与类模板

### Keywords

Generics（Java）,Template,Function Template,Class Template,Template Bloat

### 一、 模板（Template）与 Java 泛型的本质区别

- **Java 泛型**：**“类型擦除”**。在编译后全部擦除为 `Object`，运行时靠隐式强转。有运行时开销。
- **C++ 模板**：**“编译期代码生成（元编程）”**。编译器在编译阶段根据调用情况，自动复制粘贴代码并替换类型（实例化）。**零运行损耗，但会导致编译出的程序二进制体积变大（模板膨胀）**。

### 二、 函数模板与类模板

- **函数模板**：

  ```cpp
  template <typename T>
  void mySwap(T& a, T& b);
  ```

  调用时可以显式指定类型 `mySwap<double>(a, b)`，也可以由编译器自动推导类型。

- **类模板**：

  ```cpp
  template <typename T>
  class Box { T item; };
  ```

  声明对象时必须（或通常需要）显式用 `<类型>` 指明内部包装的数据类型，如 `Box<int>`。

### 三、 编译期检查

- 模板函数的某些操作（如 `>`、`+`）对代入的类型有潜在要求。
- 如果在实例化时，代入的自定义类型没有实现这些操作，编译器会在**编译期**报错拒绝通过，保证了运行时的绝对安全。

### Hints

一、 函数模板（Function Template）—— 自动代码生成器

我们从一个最经典的问题开始：**怎么写一个通用的“交换（swap）”函数？**

如果不学模板，你要交换两个 `int`，得写一个函数；要交换两个 `double`，得写重载；要交换两个自定义对象，还得再写一个……

```cpp
void mySwap(int& a, int& b) { int temp = a; a = b; b = temp; }
void mySwap(double& a, double& b) { double temp = a; a = b; b = temp; }
```

这显然是极其枯燥的体力活。

💡 解决方案：函数模板

我们写一个“模板”，让编译器自己去当打字员。

```cpp
// 1. 声明一个模板，T 是一个“虚拟的占位符类型”（Template Parameter）
template <typename T>
void mySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}
```

🛠️ 底层真相：编译器是怎么“疯狂复制粘贴代码”的？

当你写下：

```cpp
int x = 10, y = 20;
mySwap(x, y); // 1. 隐式实例化

double m = 1.1, n = 2.2;
mySwap<double>(m, n); // 2. 显式实例化
```

在编译阶段，C++ 编译器会顺着你的调用，在后台默默地开始**疯狂打字**：

1. 当它看到 `mySwap(x, y)` 传入的是两个 `int`，它立刻复制一份模板代码，把里面所有的 `T` 替换成 `int`，并在最终生成的二进制文件里生成一个 `void mySwap(int&, int&)` 函数。
2. 当它看到 `mySwap<double>(m, n)`，它又复制一份，把 `T` 替换成 `double`，生成一个 `void mySwap(double&, double&)` 函数。

> **🎯 物理本质：** 在编译完成后，你的程序里其实**依然存在多个重载函数**！ 模板本身是不占用任何代码区内存的，它只是设计图。只有当你调用它时，编译器才会照着设计图帮你**“实例化（Instantiation）”**出具体的函数代码。 \**代价\**：模板会使编译出来的可执行文件体积变大（所谓的**模板膨胀 Template Bloat**），但它换来了**零运行损耗（运行速度和手写重载一样快）**。

二、 类模板（Class Template）—— 通用的容器设计图

除了函数，类也可以做成模板。比如，你想写一个“盒子类（Box）”，它既能装苹果，又能装香蕉。

```cpp
template <typename T>
class Box {
private:
    T item;
public:
    Box(T item) : item(item) {}
    
    T getItem() {
        return item;
    }
};
```

使用类模板时，由于编译器在有些时候（尤其在老版本 C++ 中）无法自动推导类型，我们通常需要**显式用 `<类型>` 指明它里面装的是什么**：

```cpp
Box<int> intBox(100);     // 实例化一个装 int 的盒子
Box<std::string> strBox("Hello"); // 实例化一个装 string 的盒子
```

三、运算符重载

​    在 C++ 中，教编译器比大小的过程，叫做**运算符重载（Operator Overloading）**。它的物理本质是：**把一个运算符（如 `>`）包装成一个特殊的成员函数，当编译器遇到 `d1 > d2` 时，在底层自动将其转化为对这个函数的调用。**

我们来看看具体的语法和逻辑。

🛠️ 第一步：如何写大于号 `>` 的重载函数？

我们在 `Dummy` 类内部定义一个名为 **`operator>`** 的函数：

C++

```
class Dummy {
public:
    int id;
    Dummy(int id) : id(id) {}

    // 💡 运算符重载：教编译器两个 Dummy 怎么比大小
    // 物理语义：我（this 指针指向的对象）和 另一个对象（other）比大小，返回 true 或 false
    bool operator>(const Dummy& other) const {
        // 核心规则：我们自己指定，谁的 id 大，谁就更大！
        return this->id > other.id; 
    }
};
```

 🔍 语法微观拆解：

- **`bool`**：返回值。比大小的结果显然只有“真（`true`）”和“假（`false`）”。
- **`operator>`**：这是一个整体，是 C++ 的关键字，代表我们要重载“大于号”。
- **`(const Dummy& other)`**：我们要和谁比？传入另一个 `Dummy` 对象的常量引用（为了安全和高效，不进行拷贝）。
- **`const`（函数尾部的）**：这是一个极其严谨的学术规范。它向编译器承诺：**“我这个比大小的函数纯粹是只读的，绝对不会偷偷去修改 `this` 对象和 `other` 对象的任何成员变量。”**
- **`this->id > other.id`**：核心物理逻辑。顺着 `this` 拿到自己的 `id`，顺着 `other` 拿到对方的 `id`，进行底层的整数比较。

### Example Programs

1.Dummy Error

```cpp
#include <iostream>
#include <string>

// 1. 定义一个通用的打印模板
template <typename T>
void printMax(T a, T b) {
    // 💡 核心逻辑：这里使用了大于号 ">"
    if (a > b) {
        std::cout << "较大值是: " << a << std::endl;
    } else {
        std::cout << "较大值是: " << b << std::endl;
    }
}

// 2. 一个不支持比较大小的自定义类
class Dummy {
public:
    int id;
    Dummy(int id) : id(id) {}
    // 【注意】：我们没有在这个类里重载大于号 ">" 运算符！
};

int main() {
    std::cout << "--- 1. 正常模板调用 ---" << std::endl;
    printMax(10, 20);         // 编译期自动实例化为 printMax(int, int)
    printMax(3.14, 2.71);     // 编译期自动实例化为 printMax(double, double)

    std::cout << "\n--- 2. 尝试让模板处理无法处理的类型 ---" << std::endl;
    Dummy d1(1);
    Dummy d2(2);

    // 【硬核尝试】：解开下面这行的注释，点击编译（Build）！
    // printMax(d1, d2); 

    return 0;
}
```

2.Operator Overloading

```cpp
#include <iostream>

// 1. 定义一个通用的打印模板
template <typename T>
void printMax(T a, T b) {
    if (a > b) {
        std::cout << "较大值是: " << a << std::endl;
    } else {
        std::cout << "较大值是: " << b << std::endl;
    }
}

// 2. 自定义类：现在它学会了如何比大小！
class Dummy {
public:
    int id;
    Dummy(int id) : id(id) {}

    // 重载大于号运算符
    bool operator>(const Dummy& other) const {
        return this->id > other.id;
    }

    // 为了让 std::cout 能直接打印 Dummy，我们顺便重载一下输出流运算符（<<）
    // 这可以让 std::cout << a 正常工作
    friend std::ostream& operator<<(std::ostream& os, const Dummy& d) {
        os << "Dummy(id: " << d.id << ")";
        return os;
    }
};

int main() {
    std::cout << "--- 1. 让学会了比大小的 Dummy 传入模板 ---" << std::endl;
    Dummy d1(1024);
    Dummy d2(2048);

    // 💡 奇迹时刻：之前会报错的一行，现在完美编译通过！
    printMax(d1, d2); 

    return 0;
}
```

