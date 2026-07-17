#include <iostream>
#include <vector>

int main() {
    std::cout << "--- 测试 vector 扩容的地址突变 ---" << std::endl;

    std::vector<int> v;

    // 观察随着元素插入，size、capacity 以及首元素物理地址的变化
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
        
        // 打印当前的 size, capacity 以及第一个元素的内存地址
        std::cout << "放入元素 " << i 
                  << " | size = " << v.size() 
                  << " | capacity = " << v.capacity();
        
        if (!v.empty()) {
            std::cout << " | 首元素物理地址 = " << &v[0] << std::endl;
        } else {
            std::cout << std::endl;
        }
    }

    return 0;
}