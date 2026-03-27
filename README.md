# 基于嵌入式平台的在线式 EIS 锂电池安全与寿命评估系统

## 📌 项目简介
本项目提出并实现了一种基于在线式电化学阻抗谱（EIS）检测的锂电池安全与寿命评估方案。通过构建集阻抗测量、远程通信、可视化交互与 SOH（State of Health）智能预测于一体的嵌入式平台，实现了 **1 Hz -- 2 kHz** 宽频复阻抗的高精度在线检测，为锂电池的健康管理提供核心数据支撑。

## 🚀 核心特性
- **宽频复阻抗检测**：覆盖 1Hz - 2kHz 频段，支持单点测量与全频段扫描。
- **三层架构设计**：
  - **硬件执行层**：基于 StellarE 系列 MCU，控制 AD9959 信号发生与 AD7606/ADS1256 高精度采集。
  - **中控管理层**：负责 CAN 总线调度、LTE 远程数据上报（HTTP/JSON）及本地串口屏交互。
  - **云端/用户层**：Flask 后端任务分发与 Vue 3 实时可视化监控大屏。
- **智能分析**：集成阻抗实部/虚部计算、相位分析及电池状态趋势监控。
- **多维安全评估**：支持机械损伤预警、热失控监测及电池一致性均衡测试。

## 📂 项目结构
```text
EIS_IOT/
├── embedded-projects/          # 嵌入式源码 (C/StellarESDK)
│   ├── eis-system/             # EIS 核心测量系统 (逻辑主控)
│   │   └── 功能：频点调度、LTE 远程管理、SOH 计算、回传数据解析
│   ├── eis-subsystem/          # 硬件执行子系统 (从控执行器)
│   │   └── 功能：针对主控引脚不足的资源扩展，负责 AD9959(DDS) 发生、电池通道切换、Relay控制
│   ├── upper-system/           # 交互中控管理 (CAN 调度中心)
│   │   └── 功能：多模块 CAN 通信调度、本地串口屏 HMI 交互、自动化测试链路
│   ├── sdks/                   # 依赖的 StellarESDK 开发包
│   └── ...
├── flask-project/              # 后端服务 (Python/Flask)
│   ├── app.py                  # API 路由与云端任务队列管理
│   └── mail.py                 # 告警邮件通知模块
├── vue-project/                # 前端可视化 (Vue 3 + Vite)
│   └── src/components/         # Nyquist 图、历史趋势、电池健康分析组件
├── serial-screen/              # 硬件交互 UI 资源
└── TODO.txt                    # 项目开发规划与待办事项
```

## 🛠️ 技术栈
- **嵌入式**: C, StellarESDK, CAN Bus, SPI, AD9959, AD7606, LTE(Quectel)
- **后端**: Python, Flask, Threading/Queue
- **前端**: Vue 3, Vite, ECharts (数据可视化)
- **协议**: HTTP, JSON, Custom CAN Protocol

## 📡 关键技术实现
### 1. 信号发生与采集 (Master-Slave 分布式架构)
由于主控制器引脚资源限制，系统采用“逻辑主控-硬件从控”的分布式架构：
- **EIS-System (主控)**：下馈扫频指令（1Hz-2kHz），通过 CAN 总线发送控制字（ID: 0x7F0）至从控，并管理数据闭环。
- **EIS-Subsystem (从控)**：接收 CAN 指令，直接驱动 **AD9959 (DDS)** 产生高精度励磁信号，并控制 **ExpansionBoard** 实现多电池通道的物理切换与 Relay 保护。

### 2. 远程任务链路 (LTE & Cloud)
设备端通过集成的 LTE 模块采用轮询模型与 Flask 后端协同：
- 支持云端实时下发 `single`(单点)、`scan`(扫描)、`monit`(监控) 指令。
- 实现电池状态的在线初始化、SOH 趋势同步与异常邮件告警。

### 3. 多维可视化分析 (Vue 3)
- 前端实时获取扫频数据绘制 Nyquist 图，直观呈现电池电化学特性演变。
- 分层评估模块涵盖了机械损伤、热失控风险预测等健康管理应用场景。

## 📝 快速开始
### 1. 嵌入式端
- 使用 StellarStudio 导入 `embedded-projects` 下的工程。
- 编译并烧录至开发板，确保 CAN 总线连接正常。
### 2. 后端部署
```bash
cd flask-project
python app.py
```
### 3. 前端启动
```bash
cd vue-project
pnpm install
pnpm dev
```

## 📈 待办事项
- [ ] 完善 SOH 智能预测算法模型。
- [ ] 优化扫频精度与噪声滤波算法。
- [ ] 增加更多电池失效模式的特征匹配。

---
© 2024-2026 基于在线式EIS的锂电池管理项目组
