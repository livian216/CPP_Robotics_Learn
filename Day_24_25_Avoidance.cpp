#include <iostream>
#include <vector>

struct vector2D {
    double x;
    double y;
};

int main() {
    vector2D dronePos = {0.0, 0.0};
    vector2D obstaclePos = {2.0, 2.0}; // 障碍物在右前方

    double safeDistance = 5.0;

    // 最简单的距离平方计算（避免开根号，提高效率）
    double dx = dronePos.x - obstaclePos.x;
    double dy = dronePos.y - obstaclePos.y;

    double distSq = dx * dx + dy * dy;

    // --- 避障逻辑（简化版人工势场） ---
    std::cout << "检测环境..." << std::endl;

    if (distSq < safeDistance * safeDistance) {
        std::cout << "警告：发现障碍物！" << std::endl;

        // 产生斥力：向障碍物相反方向移动
        // 归一化向量逻辑略去，简化为反向
        double avoidX = -dx;
        double avoidY = -dy;

        std::cout << "生成避障向量：[" << avoidX << "," << avoidY << "]" << std::endl;
        std::cout << "无人机正在向反方向机动..." << std::endl;
    } else {
        std::cout << "前方路径畅通。" << std::endl;
    }

    return 0;
}