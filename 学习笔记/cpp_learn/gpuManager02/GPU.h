#pragma once

#include <string>
#include <iostream>

//1.gpu abstract base class
class GPU {
    protected:
        std::string modelName;
        int vramGB;

    public:

    GPU(const std::string& modelName, int vramGB);

    // 虚析构函数：
    // 当我们用基类指针去 delete 子类对象时，必须把析构函数设为 virtual，
    // 否则子类的析构函数不会被调用，会导致严重的内存泄漏！
    virtual ~GPU();

    // 纯虚函数：计算该卡在 1 小时内能提供的相对算力 TFLOPS（每秒万亿次浮点运算）
    // 末尾的 = 0 标志着它是一个纯虚函数，GPU 类也因此成为了抽象类
    virtual double calculateFlops() const =0;

    virtual void printSpec() const;

    // Getters
    std::string getModelName() const { return modelName; }
    int getVram() const { return vramGB; }
};

// ==========================================
// 2. 派生子类 1：NvidiaH100
// ==========================================
class NvidiaH100 : public GPU {
private:
    bool isSXM5; // 是否为高性能 SXM5 架构（决定算力上限）

public:
    NvidiaH100(bool isSXM5 = true);

    // override 关键字（C++11 现代特性）：
    // 显式告诉编译器：“我正在重写父类的虚函数！” 
    // 如果你拼错函数名，编译器会立刻报错提示，极大增加安全性。
    double calculateFlops() const override;
    
    void printSpec() const override;
};

// ==========================================
// 3. 派生子类 2：NvidiaA100
// ==========================================
class NvidiaA100 : public GPU {
private:
    int pciegen; // PCIe 版本（Gen4 或 Gen5）

public:
    NvidiaA100(int pciegen = 4);

    double calculateFlops() const override;
    
    void printSpec() const override;
};
