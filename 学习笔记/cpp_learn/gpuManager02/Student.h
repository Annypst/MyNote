//old style
// #ifndef STUDENT_H   // 1. If Not Defined：如果编译器在后台还没见过 STUDENT_H 这个暗号
// #define STUDENT_H   // 2. 那好，我们立刻在后台给这个暗号“挂牌登记”
#pragma once  //only import once

#include <string>

class Student {
    private:
        int id;
        std::string name;
        double score;

    public:
        //constructor
        Student(int id, std::string name, double score);

        //getters
        int getId() const ;
        std::string getName() const ;
        double getScore () const ;

        //serialize
        std::string serialize() const ;
};

// #endif              // 3. 结束判断