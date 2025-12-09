#include <iostream>
#include <thread> // 线程库
#include <chrono> // 时间库
#include <mutex> // 互斥锁库
#include <atomic> // 原子变量库


// 共享数据区
struct SharedData {
    double altitude = 0.0;
    std::mutex mtx; // 保护这块数据的锁
};


SharedData droneState;
std::atomic<bool> isRunning(true); // 线程安全的布尔标志，用于控制退出

// 线程 1： 模拟传感器 （每 100ms 更新一次高度）
void sensorLoop() {
    double dummyHeight = 0.0;
    while (isRunning) {
        {
            // 上锁： 在我写入数据时， 别人不能读写
            std::lock_guard<std::mutex> lock(droneState.mtx);
            droneState.altitude = dummyHeight;
        } // 出了花括号， 锁自动释放 （RAII特性）

        dummyHeight += 0.5; // 模拟高度变化
        std::cout << "[传感器] 更新高度：" << dummyHeight << std::endl;

        // 模拟硬件延迟 100ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



// 线程2： 模拟飞行控制算法 （每 200ms 读取一次）
void controlLoop() {
    while (isRunning) {
        double currentAlt;

        {
            // 上锁： 确保读取时，数据没有被改写一般
            std::lock_guard<std::mutex> lock(droneState.mtx);
            currentAlt = droneState.altitude;
        }


        if (currentAlt > 10.0) {
            std::cout << ">>> [控制器] 达到目标高度， 准备悬停！" << std::endl;
        } else {
            std::cout << ">>> [控制器] 正在爬升， 当前：" << currentAlt << " m" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

}




    int main() {
        std::cout << "启动多线程无人机系统..." << std::endl;


        // 启动两个子线程
        std::thread t1(sensorLoop);
        std::thread t2(controlLoop);


        // 主线程等待 2 秒
        std::this_thread::sleep_for(std::chrono::seconds(2));


        // 停止两个子线程
        isRunning = false;

        // 等待两个子线程安全结束
        t1.join();
        t2.join();


        std::cout << "系统安全关闭。" << std::endl;

        return 0;
    }