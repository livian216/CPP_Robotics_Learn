#include <iostream>
#include <unistd.h>

int main() {
    double batteryLevel = 15.0; // 假设当前电量 15%
    bool gpsLocked = true;

    // --- Day 3: If/Else 逻辑 ---
    std::cout << "--- 起飞前状态检查 ---" << std::endl;

    // 逻辑与 （&&）：两个条件都必须满足
    if (batteryLevel > 20.0 && gpsLocked) {
        std::cout << "状态：允许起飞" << std::endl;
    } else if (batteryLevel <= 20.0) {
        std::cout << "警告：电量过低，无法起飞" << std::endl;
    } else {
        std::cout << "警告：GPS未锁定，无法起飞" << std::endl;
    }



    // --- Day 4: For/While 循环 ---
    std::cout << "\n--- 模拟点击预热 ---" << std::endl;
    // 倒计时循环
    for (int i = 3; i > 0; i--) {
        std::cout << "倒计时：" << i << std::endl;
        sleep(1);
    }
    std::cout << "电机启动！\n" << std::endl;

    // 模拟传感器数据采集（while循环）
    int sampleCount = 0;
    while(sampleCount < 5){
        std::cout << "正在读取第：" << sampleCount + 1 << "次IMU数据..." << std::endl;
        sampleCount++;
        sleep(1);
    }

    return 0;
}