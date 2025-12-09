#include <iostream>
#include <cmath> // 用于 sin, cos

// 定义 PI
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

struct Vector3 {
    double x, y, z;
};


class Transform {
    public:
        // 将角度转换为弧度
        static double deg2rad(double degrees) {
            return degrees * M_PI / 180.0;
        }


        // 核心算法： 2D 旋转矩阵 （绕 Z 轴旋转）
        // 输入：机体坐标 （bodyFrame）， 无人机朝向 （yaw 角度）
        // 输出：世界坐标 （worldFrame）
        static Vector3 bodyToworld(Vector3 body, double yaw) {
            double theta = deg2rad(yaw);
            double cosT = std::cos(theta);
            double sinT = std::sin(theta);

            Vector3 world;
            // 旋转矩阵公式：
            // x' = x * cosT - y * sinT
            // y' = x * sinT + y * cosT
            world.x = body.x * cosT - body.y * sinT;
            world.y = body.x * sinT + body.y * cosT;
            world.z = body.z; // Z 轴不变


            return world;
        }
};



int main() {
    // 假设无人机传感器显示，前方 10 米有一个障碍物
    Vector3 obstacleBody = {10.0, 0.0, 5.0};

    // 假设无人机当前朝向： 偏航角 90 度 （面向正北/正Y方向）
    double currentYaw = 90.0;

    // 计算障碍物在地图上的绝对坐标
    Vector3 obstacleWorld = Transform::bodyToworld(obstacleBody, currentYaw);

    std::cout << "--- 坐标变换测试 ---" << std::endl;
    std::cout << "机体坐标：[" << obstacleBody.x << ", " << obstacleBody.y << ", " << obstacleBody.z << "]" << std::endl;
    std::cout << "偏航角：" << currentYaw << " 度" << std::endl;

    // 预期结果：如果机头朝北（Y轴），前方 10 米应该是世界坐标的（0,10）
    std::cout << "世界坐标：[" << obstacleWorld.x << ", " << obstacleWorld.y << ", " << obstacleWorld.z << "]" << std::endl;

    return 0;
}