#include <iostream>
#include <vector>
#include <cmath>

// --- Day 6: 函数定义 ---
// 计算两点间距离的函数：sqrt((x1 - x2)^2 + (y1 - y2)^2)
double calculateDistance(double x1, double y1, double x2, double y2) {
    double dx = x1 -x2;
    double dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}


int main() {
    // ---Day 5: std::vector 容器---
    // vector 是一个可以动态增长的数组，非常适合存储不确定数量的航点
    std::vector<double> x_coords = {0.0, 10.5, 20.0};
    std::vector<double> y_coords = {0.0, 5.5, 15.0};

    // 添加一个新的航点
    x_coords.push_back(30.0);
    y_coords.push_back(25.0);

    std::cout << "--- 航点距离计算 ---" << std::endl;

    // size() 获取元素个数
    for (size_t i = 0; i < x_coords.size() - 1; i++) {
        double dist = calculateDistance(x_coords[i], y_coords[i], x_coords[i + 1], y_coords[i + 1]);

        std::cout << "从航点" << i << "到" << i + 1 << "的距离：" << dist << "米" <<std::endl;
    }

    return 0;
}