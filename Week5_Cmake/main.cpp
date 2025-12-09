#include <iostream>
#include "VectorMath.h"

int main() {
    std::cout << "-- 模块化编译测试 ---" << std::endl;

    Vec2 v1 = {1.0, 2.0};
    Vec2 v2 = {3.0, 4.0};

    Vec2 v3 = v1.add(v2);

    std::cout << "v1 + v2 = ";
    v3.print();


    return 0;
}