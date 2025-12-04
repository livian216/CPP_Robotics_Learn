#include <iostream>
#include <string>

int  main() {
    // --- Day 1: 输入输出 ---
    std::cout << "=== 无人机系统初始化 ===" << std::endl;

    // --- Day 2: 变量定义 ---
    // double 用于存储小数，如坐标、高度、电压
    double altitude = 0.0;
    double voltage = 12.6;

    // bool 用于存储状态 (true/false)
    bool isArmed = false; // 是否解锁电机

    // string 用于存储名称
    std::string droneName = "Explorer-V1";

    std::cout << "无人机名称：" << droneName << std::endl;
    std::cout << "当前电压：" << voltage <<std::endl;
    std::cout << "请输入起飞目标高度（米）：";


    // 获取用户输入
    double targetHeight;
    std::cin >> targetHeight;

    std::cout << "指令接收：准备爬升至" << targetHeight << "米。" << std::endl;

    return 0;

}