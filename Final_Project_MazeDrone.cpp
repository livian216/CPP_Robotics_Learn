#include <iostream>
#include <vector>
#include <cmath>
#include <thread> // 用于延迟
#include <chrono> // 用于时间

// 定义地图大小
const int MAP_SIZE = 10;

// 简单的点结构
struct Point {
    int x;
    int y;

    // 重加载 == 运算符，方便比较
    bool operator==(const Point& other) {
        return x == other.x && y == other.y;
    }
};


class DroneSimulator {
    private:
        char grid[MAP_SIZE][MAP_SIZE];
        Point dronePos;
        Point targetPos;


    public:
        DroneSimulator() {
            // 1. 初始化地图（.为空地，#为障碍物）
            for (int i = 0; i < MAP_SIZE; i++) {
                for (int j = 0; j < MAP_SIZE; j++) {
                    grid[i][j] = '.';
                }
            }

            // 2. 设置一些障碍物
            grid[3][2] = '#'; grid[3][3] = '#'; grid[3][4] = '#';
            grid[6][6] = '#'; grid[5][6] = '#'; grid[4][6] = '#';

            dronePos = {0, 0};
            targetPos = {8, 8};

            grid[targetPos.x][targetPos.y] = 'T'; // T 代表目标
        }


        void render() {
            // 清屏 （Windows 用cls，Linux 用clear）
            std::system("clear");
            // 这里为了简单直接打印换行
            // std::cout << std::string(5, '\n');


            for (int y = 0; y < MAP_SIZE; y++) { 
                for (int x = 0; x < MAP_SIZE; x++) {
                    if (x == dronePos.x && y == dronePos.y) {
                        std::cout << "A"; // A 代表无人机 （Aircraft）
                    } else {
                        std::cout << grid[x][y] << " ";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << "Target:(" << targetPos.x << "," << targetPos.y << ")" << std::endl;
        }



        // 简单的贪心算法：哪边离目标近且没有障碍物，就往那边走
        void update() {
            if (dronePos == targetPos) {
                std::cout << "Mission Complete!" << std::endl;
                exit(0);
            }

            Point bestMove = dronePos;
            double minDist = 9999.0;


            int dx[] = {0, 0, -1, 1};
            int dy[] = {-1, 1, 0, 0};

            for (int i = 0; i < 4; i++) {
                Point next = {dronePos.x + dx[i], dronePos.y + dy[i]};

                // 检查边界
                if (next.x < 0 || next.x >= MAP_SIZE || next.y < 0 || next.y >= MAP_SIZE) {
                    continue;
                }

                // 检查是否可走
                if (grid[next.x][next.y] == '#') {
                    continue;
                }

                double Dist = std::sqrt(std::pow(next.x - targetPos.x, 2) + std::pow(next.y - targetPos.y, 2));

                if (Dist < minDist) {
                    minDist = Dist;
                    bestMove = next;
                }

            }

            // 移动无人机
            dronePos = bestMove;
        }

        void run() {
            while (true) {
                render();
                update();
                // 暂停 0.5 秒
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
};




int main() {
    DroneSimulator sim;
    sim.run();
    return 0;
}