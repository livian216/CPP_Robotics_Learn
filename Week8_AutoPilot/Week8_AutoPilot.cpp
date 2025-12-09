#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>


// --- 基础数学工具 ---
struct Vec2 {
    double x, y;

    Vec2(double x, double y) : x(x), y(y) {}

    Vec2 operator+(const Vec2 &other) {
        return {x + other.x, y + other.y};
    }

    Vec2 operator-(const Vec2 &other) {
        return {x - other.x, y - other.y};
    }

    // 标量乘法
    Vec2 operator*(double s) {
        return {x * s, y * s};
    }

    double length() {
        return std::sqrt(x*x + y*y);
    }

    // 归一化 （变成长度为 1 的单位向量）
    Vec2 normalize() {
        double l = length();
        if (l == 0) return {0, 0};
        return {x / l, y / l};
    }
};




// --- 仿真环境对象 ---
class Drone {
    public:
        Vec2 position;
        Vec2 velocity;

        Drone(double x, double y) : position({x,y}), velocity({0,0}) {}

        // 物理引擎更新： 位置 = 旧位置 + 速度 * 时间
        void updatePhysics(double dt) {
            position = position + (velocity * dt);
        }

};


class Obstacle {
    public:
        Vec2 position;
        double radius;
};


// --- 核心算法： 人工势场法 （Artificial Potential Field） ---
// 原理： 目标点产生引力，障碍物产生斥力
Vec2 calculateControlForce(Drone &drone, Vec2 target, Obstacle &obstacle) {
    // 1. 引力 （Attraction）： 朝向目标
    Vec2 toTarget = target - drone.position;
    Vec2 attraction = toTarget.normalize() * 1.0; // 引力系数 1.0

    // 2. 斥力 （Repulsion）： 远离障碍物
    Vec2 toObstacle = drone.position - obstacle.position; // 障碍物指向无人机的向量
    double dist = toObstacle.length();
    Vec2 repulsion = {0.0, 0.0};

    // 只有距离小于安全距离 （比如 3 米） 时才产生斥力
    if (dist < 3.0) {
        // 距离越近，斥力越大 （1/distance）
        double strength = 5.0 / (dist * dist);
        repulsion = toObstacle.normalize() * strength;
        std::cout << "[避障修正！]";
    }

    // 合力
    return attraction + repulsion;
}



int main() {
    std::cout << "开始模拟！" << std::endl;

    Drone myDrone(0.0, 0.0); // 起点 （0,0）
    Vec2 target(10.0, 10.0); // 目标点 （10,10）
    Obstacle wall = {{5.0,5.0}, 1.0}; // 障碍物 5,5 半径1.0


    double dt = 0.1; // 时间步长 0.1 秒

    std::cout << "--- 任务开始：（0,0） -> (10,10) ---" << std::endl;

    for (int i = 0; i < 200; i++) {
        // 1. 计算控制指令 （算法层）
        Vec2 force = calculateControlForce(myDrone, target, wall);

        // 2. 更新速度 （假设力直接改变速度，简化物理模型）
        // 限制最大速度
        myDrone.velocity = force;

        // 3. 物理更新 （仿真层）
        myDrone.updatePhysics(dt);

        // 4. 打印遥测数据
        printf("Time: %.1fs | Pos: (%.2f, %.2f) | Dist to Target: %.2f\n", i*dt, myDrone.position.x, myDrone.position.y, (target - myDrone.position).length());

        // 判断是否到达
        if ((target - myDrone.position).length() < 0.5) {
            std::cout << "--- 任务完成： 到达目标点！" << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    return 0;
}