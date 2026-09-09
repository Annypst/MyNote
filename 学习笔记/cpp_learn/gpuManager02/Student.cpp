#include "Student.h"
#include <sstream>

// 构造函数初始化列表
Student::Student(int id, std::string name, double score) 
    : id(id), name(name), score(score) {}

int Student::getId() const { return id; }
std::string Student::getName() const { return name; }
double Student::getScore() const { return score; }

// 将数据转为 "学号 姓名 成绩" 的空格分隔格式
std::string Student::serialize() const {
    std::stringstream ss;
    ss << id << " " << name << " " << score;
    return ss.str();
}