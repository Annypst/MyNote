#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Student.h"
#include "GPU.h"
#include <algorithm>

const std::string FILE_NAME = "students.txt";

// 物理持久化：把内存中的 vector 写入硬盘文件
void saveToFile(const std::vector<Student>& students) {
    // std::ofstream: Output File Stream（输出文件流，内存 -> 硬盘）
    std::ofstream outFile(FILE_NAME);
    
    if (!outFile.is_open()) {
        std::cerr << "【错误】无法打开文件进行写入！" << std::endl;
        return;
    }

    for (const auto& stu : students) {
        outFile << stu.serialize() << "\n"; // 按行写入
    }
    
    outFile.close(); // 记得物理关闭文件流
    std::cout << "» 数据成功保存到硬盘 (" << FILE_NAME << ")" << std::endl;
}

// 数据恢复：从硬盘文件读取，加载回内存的 vector 中
void loadFromFile(std::vector<Student>& students) {
    // std::ifstream: Input File Stream（输入文件流，硬盘 -> 内存）
    std::ifstream inFile(FILE_NAME);
    
    if (!inFile.is_open()) {
        std::cout << "» 未检测到存档文件，将创建全新数据库。" << std::endl;
        return;
    }

    students.clear(); // 清空当前内存
    int id;
    std::string name;
    double score;

    // 顺着文件一行行读取，直到文件末尾 (EOF)
    while (inFile >> id >> name >> score) {
        students.push_back(Student(id, name, score));
    }

    inFile.close();
    std::cout << "» 成功从硬盘加载了 " << students.size() << " 个学生数据！" << std::endl;
}

int main() {
    std::vector<Student> db;

    std::cout << "=== 算力资源管理系统 v1.0 启动 ===" << std::endl;
    // 启动时，先物理加载一次存档
    loadFromFile(db);

    // 如果内存为空，模拟录入几条初始数据
    if (db.empty()) {
        std::cout << "» 正在初始化默认测试数据..." << std::endl;
        db.push_back(Student(1001, "Zhou", 95.5));
        db.push_back(Student(1002, "TeacherWu", 99.0));
        db.push_back(Student(1003, "Alice", 88.5));
        // 立刻存盘
        saveToFile(db);
    }

    // 打印当前内存中的学生
    std::cout << "\n--- 当前系统内学生列表 ---" << std::endl;
    for (const auto& stu : db) {
        std::cout << "学号: " << stu.getId() 
                  << " | 姓名: " << stu.getName() 
                  << " | 综合表现分: " << stu.getScore() << std::endl;
    }

    std::cout << "\n=== 算力集群物理设备检测（多态演示） ===" << std::endl;

    std::vector<GPU*> cluster;
    // 1. 用基类指针数组，统一管理不同的子类硬件对象！
// 注意：因为 GPU 是抽象类，我们不能 new GPU()，但可以定义 GPU* (基类指针)
    cluster.push_back(new NvidiaH100(true));
    cluster.push_back(new NvidiaA100(4));
    cluster.push_back(new NvidiaA100(100000));

    //calculate the total flops
    double totalFlops = 0.0;
    for(GPU* curGPU : cluster){
        curGPU->printSpec();
        totalFlops+=curGPU->calculateFlops();
    }
    std::cout << "» 当前集群总物理算力: " << totalFlops << " TFLOPS" << std::endl;
   
   
    std::cout << "------------sort------------" << std::endl
    << std::endl;

    std::sort(cluster.begin(),cluster.end(),[](const GPU* g1, GPU* g2){return g1->calculateFlops() < g2->calculateFlops();});

    for(const GPU* curGPU : cluster){
        curGPU->printSpec();
    }


    //destory 
    for(GPU* curGPU : cluster){
        delete curGPU;
    }

 


    system("pause");
    return 0;
}