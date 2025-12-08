#include <iostream>

// 使用 enum 定义状态，比如0 表示初始状态，1 表示状态 A，2 表示状态 B，3 表示状态 C
enum DroneState {IDLE, TAKEOFF, HOVER, LAND};

int main() {
    DroneState currentState = IDLE;
    char input;
    bool running = true;
    while (running) {
        // 打印当前状态
        switch (currentState) {
            case IDLE: std::cout << "[当前状态：待机] （按 t 起飞，q 退出）:"; break;
            case TAKEOFF: std::cout << "[当前状态：起飞中...] -> 自动转为悬停\n"; currentState = HOVER; continue;
            case HOVER: std::cout << "[当前状态：悬停] （按 l 降落）:"; break;
            case LAND: std::cout << "[当前状态：降落中...] -> 自动转为待机\n"; currentState = IDLE; continue;
        }

        std::cin >> input;

        // 状态转移逻辑
        if (currentState == IDLE && input == 't') {
            currentState = TAKEOFF;
        } else if (currentState == HOVER && input == 'l') {
            currentState = LAND;
        } else if (input == 'q') {
            running = false;
        } else {
            std::cout << "无效输入！请重新输入。\n";
        }
    }

    return 0;
}