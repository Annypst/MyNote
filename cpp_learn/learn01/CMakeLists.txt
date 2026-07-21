cmake_minimum_required(VERSION 3.10)

# 项目名称
project(cpp_learn)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)

# 生成可执行文件
add_executable(cpp_learn main.cpp
    sum.cpp
    sum.h)