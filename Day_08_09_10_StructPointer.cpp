#include <iostream>

// --- Day 10: 结构体 ---
// 将 x，y，z 组合成一个“位置”概念

struct Point3D {
    double x;
    double y;
    double z;
};

// --- Day 9: 引用 ---
// const Point3D &p ：意思是“给我这个变量的别名，我不复制它，我也不修改它”
// 这在传递大型数据（如地图、图像）时非常高效

void printPoint(const Point3D &p) {
    std::cout << "Pos:[" << p.x << ", " << p.y << ", " << p.z << "]" << std::endl;
}

// --- Day 8: 指针 ---
// 指针修改变量的值
void calibrateAltitude(double *alt) {
    *alt = 0.0; // 将指针指向的地址内的值设置为0.0
}


int main() {
    Point3D currentPos = {10.5, 20.1, 50.0};

    std::cout << "原始数据：";
    printPoint(currentPos); // 传引用调用

    double rawAlt = 55.2;
    std::cout << "校准前高度：" << rawAlt << std::endl;

    // 传地址 （&rowAlt） 给指针
    calibrateAltitude(&rawAlt);
    std::cout << "校准后高度：" << rawAlt << std::endl;

    return 0;
}