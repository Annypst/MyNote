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