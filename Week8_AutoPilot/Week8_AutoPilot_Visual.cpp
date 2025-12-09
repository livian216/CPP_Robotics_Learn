#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <sys/stat.h> // 用于创建目录

// --- 基础数学工具 ---
struct Vec2 {
    double x, y;
    
    // 添加构造函数
    Vec2() : x(0), y(0) {}
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
        if (l == 0) return Vec2{0, 0};
        return Vec2{x / l, y / l};
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

// --- 地图绘制类 ---
class MapRenderer {
private:
    static const int MAP_WIDTH = 50;
    static const int MAP_HEIGHT = 50;
    char map[MAP_HEIGHT][MAP_WIDTH];
    const std::string frame_dir = "frames"; // 帧文件保存目录
    
public:
    double scale_x, scale_y;
    Vec2 origin;
    
    MapRenderer() {
        scale_x = 2.0; // 每个字符代表2米
        scale_y = 2.0;
        origin = Vec2(0, 0);
        
        // 创建帧文件保存目录
        mkdir(frame_dir.c_str(), 0777);
    }
    
    void clearMap() {
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                map[i][j] = '.';
            }
        }
    }
    
    // 将世界坐标转换为地图坐标
    std::pair<int, int> worldToMap(Vec2 pos) {
        int x = (int)((pos.x - origin.x) / scale_x) + MAP_WIDTH/2;
        int y = (int)((pos.y - origin.y) / scale_y) + MAP_HEIGHT/2;
        return {x, y};
    }
    
    // 在地图上标记点
    void markPoint(Vec2 pos, char symbol) {
        auto coords = worldToMap(pos);
        int x = coords.first;
        int y = coords.second;
        
        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
            map[y][x] = symbol;
        }
    }
    
    // 绘制障碍物
    void drawObstacle(Vec2 center, double radius) {
        // 简化处理，只标记中心点
        markPoint(center, 'O');
    }
    
    // 绘制目标点
    void drawTarget(Vec2 target) {
        markPoint(target, 'T');
    }
    
    // 显示地图
    void displayMap() {
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                std::cout << map[i][j];
            }
            std::cout << std::endl;
        }
    }
    
    // 保存地图帧到文件（用于生成动画）
    void saveFrame(int frameNumber) {
        std::stringstream filepath;
        filepath << frame_dir << "/frame_" << frameNumber << ".txt";
        std::ofstream file(filepath.str());
        
        if (file.is_open()) {
            for (int i = 0; i < MAP_HEIGHT; i++) {
                for (int j = 0; j < MAP_WIDTH; j++) {
                    file << map[i][j];
                }
                file << std::endl;
            }
            file.close();
        }
    }
    
    // 保存带有图例的数据帧（用于Python动画）
    void saveDataFrame(int frameNumber, const std::vector<Vec2>& trajectory, const Vec2& dronePos, const Vec2& targetPos, const Obstacle& obstacle) {
        std::stringstream filepath;
        filepath << frame_dir << "/data_frame_" << frameNumber << ".csv";
        std::ofstream file(filepath.str());
        
        if (file.is_open()) {
            // 写入头部信息
            file << "type,x,y\n";
            
            // 写入轨迹点
            for (const auto& point : trajectory) {
                file << "path," << point.x << "," << point.y << "\n";
            }
            
            // 写入当前位置
            file << "drone," << dronePos.x << "," << dronePos.y << "\n";
            
            // 写入目标点
            file << "target," << targetPos.x << "," << targetPos.y << "\n";
            
            // 写入障碍物
            file << "obstacle," << obstacle.position.x << "," << obstacle.position.y << "\n";
            
            file.close();
        }
    }
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
    if (dist < 3.0 && dist > 0.1) { // 添加最小距离检查防止除零
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
    Vec2 target(10.0, 7.5); // 目标点 （10,7.5）
    Obstacle wall = {{5.0,5.0}, 1.0}; // 障碍物 5,5 半径1.0
    
    // 创建地图渲染器
    MapRenderer renderer;
    
    // 记录飞行轨迹点
    std::vector<Vec2> trajectory;

    double dt = 0.1; // 时间步长 0.1 秒
    int frameCount = 0;

    std::cout << "--- 任务开始：（0,0） -> (10,10) ---" << std::endl;

    for (int i = 0; i < 200; i++) {
        // 1. 计算控制指令 （算法层）
        Vec2 force = calculateControlForce(myDrone, target, wall);

        // 2. 更新速度 （假设力直接改变速度，简化物理模型）
        // 限制最大速度
        myDrone.velocity = force;

        // 3. 物理更新 （仿真层）
        myDrone.updatePhysics(dt);
        
        // 记录轨迹点
        trajectory.push_back(myDrone.position);

        // 每5步绘制一次地图
        if (i % 5 == 0) {
            // 清空地图
            renderer.clearMap();
            
            // 绘制障碍物
            renderer.drawObstacle(wall.position, wall.radius);
            
            // 绘制目标点
            renderer.drawTarget(target);
            
            // 绘制轨迹
            for (const auto& point : trajectory) {
                renderer.markPoint(point, '*');
            }
            
            // 绘制无人机当前位置
            renderer.markPoint(myDrone.position, 'D');
            
            // 显示地图
            std::cout << "\033[2J\033[1;1H"; // 清屏并移动光标到左上角
            std::cout << "时间: " << i*dt << "s" << std::endl;
            std::cout << "无人机位置: (" << myDrone.position.x << ", " << myDrone.position.y << ")" << std::endl;
            std::cout << "距离目标: " << (target - myDrone.position).length() << " 米" << std::endl;
            renderer.displayMap();
            
            // 保存帧用于动画制作
            renderer.saveFrame(frameCount);
            
            // 保存数据帧用于Python动画
            renderer.saveDataFrame(frameCount, trajectory, myDrone.position, target, wall);
            
            frameCount++;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        // 4. 打印遥测数据
        printf("Time: %.1fs | Pos: (%.2f, %.2f) | Dist to Target: %.2f\n", i*dt, myDrone.position.x, myDrone.position.y, (target - myDrone.position).length());

        // 判断是否到达
        if ((target - myDrone.position).length() < 0.5) {
            std::cout << "--- 任务完成： 到达目标点！" << std::endl;
            break;
        }
    }
    
    // 生成Python脚本用于创建动画
    std::ofstream script("create_animation.py");
    if (script.is_open()) {
        script << "import os\n";
        script << "# 设置matplotlib后端\n";
        script << "import matplotlib\n";
        script << "matplotlib.use('Agg')  # 使用非GUI后端\n";
        script << "import matplotlib.pyplot as plt\n";
        script << "import matplotlib.animation as animation\n";
        script << "import pandas as pd\n";
        script << "import numpy as np\n\n";
        script << "def read_frame_data(filename):\n";
        script << "    return pd.read_csv(filename)\n\n";
        script << "# 获取所有数据帧文件\n";
        script << "data_frames = sorted([f for f in os.listdir('frames') if f.startswith('data_frame_') and f.endswith('.csv')])\n\n";
        script << "# 创建图形\n";
        script << "fig, ax = plt.subplots(figsize=(10, 10))\n";
        script << "ax.set_xlim(-5, 15)\n";
        script << "ax.set_ylim(-5, 15)\n";
        script << "ax.set_xlabel('X (m)')\n";
        script << "ax.set_ylabel('Y (m)')\n";
        script << "ax.grid(True)\n\n";
        script << "# 初始化绘图元素（使用英文标签）\n";
        script << "path_line, = ax.plot([], [], 'b-', linewidth=1, label='Flight Path')\n";
        script << "drone_point, = ax.plot([], [], 'ro', markersize=8, label='Drone')\n";
        script << "target_point, = ax.plot([], [], 'g*', markersize=12, label='Target')\n";
        script << "obstacle_point, = ax.plot([], [], 'ko', markersize=10, label='Obstacle')\n";
        script << "ax.legend()\n\n";
        script << "def update_frame(i):\n";
        script << "    # 读取数据\n";
        script << "    df = read_frame_data(os.path.join('frames', data_frames[i]))\n\n";
        script << "    # 提取各类点\n";
        script << "    path_points = df[df['type'] == 'path']\n";
        script << "    drone_points = df[df['type'] == 'drone']\n";
        script << "    target_points = df[df['type'] == 'target']\n";
        script << "    obstacle_points = df[df['type'] == 'obstacle']\n\n";
        script << "    # 更新轨迹线\n";
        script << "    if len(path_points) > 0:\n";
        script << "        path_line.set_data(path_points['x'], path_points['y'])\n\n";
        script << "    # 更新无人机位置\n";
        script << "    if len(drone_points) > 0:\n";
        script << "        drone_point.set_data(drone_points['x'].iloc[0], drone_points['y'].iloc[0])\n\n";
        script << "    # 更新目标点\n";
        script << "    if len(target_points) > 0:\n";
        script << "        target_point.set_data(target_points['x'].iloc[0], target_points['y'].iloc[0])\n\n";
        script << "    # 更新障碍物\n";
        script << "    if len(obstacle_points) > 0:\n";
        script << "        obstacle_point.set_data(obstacle_points['x'].iloc[0], obstacle_points['y'].iloc[0])\n\n";
        script << "    ax.set_title(f'Drone Flight Path - Time: {i*0.5:.1f}s')\n";
        script << "    return path_line, drone_point, target_point, obstacle_point\n\n";
        script << "# 创建动画\n";
        script << "ani = animation.FuncAnimation(fig, update_frame, frames=len(data_frames), interval=500, blit=True)\n";
        script << "plt.tight_layout()\n\n";
        script << "# 保存为GIF文件而不是显示\n";
        script << "ani.save('drone_flight.gif', writer='pillow', fps=2)\n";
        script << "print(\"动画已保存为 drone_flight.gif\")\n";
        script.close();
        std::cout << "\n已生成动画脚本 create_animation.py" << std::endl;
        std::cout << "运行 'python3 create_animation.py' 查看动画效果" << std::endl;
        std::cout << "或者直接运行 'python3 create_animation.py' 生成 GIF 动画文件" << std::endl;
    }

    return 0;
}