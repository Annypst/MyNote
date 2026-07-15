# 1

### Keywords



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

