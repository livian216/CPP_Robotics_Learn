import os
# 设置matplotlib后端
import matplotlib
matplotlib.use('Agg')  # 使用非GUI后端
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import numpy as np

def read_frame_data(filename):
    return pd.read_csv(filename)

# 获取所有数据帧文件
data_frames = sorted([f for f in os.listdir('frames') if f.startswith('data_frame_') and f.endswith('.csv')])

# 创建图形
fig, ax = plt.subplots(figsize=(10, 10))
ax.set_xlim(-5, 15)
ax.set_ylim(-5, 15)
ax.set_xlabel('X (m)')
ax.set_ylabel('Y (m)')
ax.grid(True)

# 初始化绘图元素（使用英文标签）
path_line, = ax.plot([], [], 'b-', linewidth=1, label='Flight Path')
drone_point, = ax.plot([], [], 'ro', markersize=8, label='Drone')
target_point, = ax.plot([], [], 'g*', markersize=12, label='Target')
obstacle_point, = ax.plot([], [], 'ko', markersize=10, label='Obstacle')
ax.legend()

def update_frame(i):
    # 读取数据
    df = read_frame_data(os.path.join('frames', data_frames[i]))

    # 提取各类点
    path_points = df[df['type'] == 'path']
    drone_points = df[df['type'] == 'drone']
    target_points = df[df['type'] == 'target']
    obstacle_points = df[df['type'] == 'obstacle']

    # 更新轨迹线
    if len(path_points) > 0:
        path_line.set_data(path_points['x'], path_points['y'])

    # 更新无人机位置
    if len(drone_points) > 0:
        drone_point.set_data(drone_points['x'].iloc[0], drone_points['y'].iloc[0])

    # 更新目标点
    if len(target_points) > 0:
        target_point.set_data(target_points['x'].iloc[0], target_points['y'].iloc[0])

    # 更新障碍物
    if len(obstacle_points) > 0:
        obstacle_point.set_data(obstacle_points['x'].iloc[0], obstacle_points['y'].iloc[0])

    ax.set_title(f'Drone Flight Path - Time: {i*0.5:.1f}s')
    return path_line, drone_point, target_point, obstacle_point

# 创建动画
ani = animation.FuncAnimation(fig, update_frame, frames=len(data_frames), interval=500, blit=True)
plt.tight_layout()

# 保存为GIF文件而不是显示
ani.save('drone_flight.gif', writer='pillow', fps=2)
print("动画已保存为 drone_flight.gif")
