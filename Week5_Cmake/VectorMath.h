// #ifndef 防止头文件被多次引用导致编译错误

#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

// 定义一个简单的二维向量结构体
struct Vec2 {
    double x;
    double y;

    // 声明函数：向量加法
    Vec2 add(const Vec2 &other);
    // 声明函数：打印向量
    void print();
};

#endif