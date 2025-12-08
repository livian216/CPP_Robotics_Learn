#include <iostream>
#include <string>

// --- Day 17: 基类（父类） ---
class Sensor {
    protected:
        std::string name;
    public:
        Sensor(std::string n) : name(n) {}

        // --- Day 18: 虚函数（多态） ---
        // virtual 意味着：如果不确定是哪个传感器，就看具体是哪个子类在运行
        virtual void readData() {
            std::cout << "读取通用传感器数据..." << std::endl;
        }

        std::string getName() {
            return name;
        }

        virtual ~Sensor() {} // 虚析构函数（好习惯）
};

// 派生类：GPS
class GPS : public Sensor {
    public:
        GPS() : Sensor("GPS-M8N") {}

        // 重写基类的方法
        void readData() override {
            std::cout << "[" << name << "] 锁定卫星：32颗，位置：（30.1， 120.2）" << std::endl;
        }
};


// 派生类：Lidar
class Lidar : public Sensor {
    public:
        Lidar() : Sensor("RPLidar-A1") {}

        void readData() override {
            std::cout << "[" << name << "] 前方障碍物距离：2.5米" << std::endl;
        }
};


// --- Day 15 & 16: 封装无人机类 ---
class Drone {
    private:
        bool armed; // 私有变量，外部不能直接修改

    public:
        Drone() : armed(false) {}

        void arm() {
            armed = true;
            std::cout << "无人机已解锁" << std::endl;
        }

        // 接受任何 sensor 的子类 （多态的威力）
        void checkSensor(Sensor* s) {
            std::cout << "正在检查传感器：" << s->getName() << std::endl;
            s->readData(); // 程序会自动判断是调用 GPS 的 readData 还是 Lidar 的
        }
};

int main() {
    Drone mydrone;
    mydrone.arm();

    GPS myGps;
    Lidar myLidar;

    // 统一接口调用不同的硬件逻辑
    mydrone.checkSensor(&myGps);
    mydrone.checkSensor(&myLidar);

    return 0;
}