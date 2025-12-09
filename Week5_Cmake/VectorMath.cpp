#include <iostream>
#include "VectorMath.h"

// 实现 add 函数
// Vec2:: 表示这个函数属于 Vec2 结构体

Vec2 Vec2::add(const Vec2 &other) {
    Vec2 result;
    result.x = x + other.x;
    result.y = y + other.y;
    return result;
}


// 实现 print 函数
void Vec2::print() {
    std::cout << "Vector(" << x << "," << y << ")" << std::endl;
}