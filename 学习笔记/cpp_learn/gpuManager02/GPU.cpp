#include "GPU.h"

GPU::GPU(const std::string& modelName, int vramGB) 
    : modelName(modelName), vramGB(vramGB) {};

GPU::~GPU() {
    std::cout<<"GPU: "<<this->modelName<<" is destroyed!"
             <<std::endl;
}

void GPU::printSpec() const {
    std::cout << "硬件型号: " << modelName << " | 显存容量: " 
    << vramGB << " GB" <<std::endl;
}


// ------------------------------------------
// NvidiaH100 子类实现
// ------------------------------------------
// 这里的 : GPU("NVIDIA H100 Tensor Core", 80) 是“构造函数委托”
// 子类必须显式调用父类的构造函数，先把父类那部分内存初始化好！
NvidiaH100::NvidiaH100(bool isSXM5 )
    : GPU("NVIDIA H100 Tensor Core", 80), isSXM5(isSXM5) {}

double NvidiaH100::calculateFlops() const {
    // SXM5 版本的 H100 算力可达 2000 TFLOPS (FP16)，PCIe 版约为 1500
    return isSXM5 ? 2000.0 : 1500.0;
}

void NvidiaH100::printSpec() const {
    GPU::printSpec(); // 先调用父类的基础打印
    std::cout << " | 架构: " << (isSXM5 ? "SXM5 (极速总线)" : "PCIe") 
              << " | 算力: " << calculateFlops() << " TFLOPS" << std::endl;
}

// ------------------------------------------
// NvidiaA100 子类实现
// ------------------------------------------
NvidiaA100::NvidiaA100(int pciegen) 
    : GPU("NVIDIA A100 Tensor Core", 40), pciegen(pciegen) {}

double NvidiaA100::calculateFlops() const {
    // A100 基础算力约 624 TFLOPS
    return 624.0 * (pciegen == 5 ? 1.1 : 1.0);
}

void NvidiaA100::printSpec() const {
    GPU::printSpec();
    std::cout << " | 总线: PCIe Gen" << pciegen 
              << " | 算力: " << calculateFlops() << " TFLOPS" << std::endl;
}