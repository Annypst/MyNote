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