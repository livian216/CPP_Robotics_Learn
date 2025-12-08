#include <iostream>
#include <fstream> // 文件流
#include <memory> // 智能指针库

struct LidarData {
    int distance[5] = {100, 120, 90, 80, 200};
};

int main() {
    // ---Day 12: 智能指针 ---
    // unique_ptr: 独占所有权，当sensor变量超出作用域（函数结束）时，自动释放内存
    // 它是替代 new/delete 的现代写法
    std::unique_ptr<LidarData> sensor = std::make_unique<LidarData>();

    std::cout << "读取雷达中心数据：" << sensor->distance[2] << "cm" << std::endl;

    // ---Day 13: 文件写入 ---
    std::ofstream logFile("flight_log.txt"); // 创建文件
    if (logFile.is_open()) {
        for (int i = 0; i < 5; i++) {
            logFile << i << "," << sensor->distance[i] << std::endl;
        }
        logFile.close();
        std::cout << "数据已写入 fight_log.txt" << std::endl;
    } else {
        std::cout << "无法打开文件！" << std::endl;
    }

    return 0; // sensor 内存通过智能指针在此处自动释放
}