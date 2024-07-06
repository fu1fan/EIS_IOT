/*
 * zf_device_imu660ra.h
 *
 *  Created on: 2024年2月28日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DEVICE_CODE_ZF_DEVICE_IMU660RA_H_
#define DEVICE_CODE_ZF_DEVICE_IMU660RA_H_

#include <config.h>
#include "zf_common_typedef.h"

#include "spi_config.h"

#define IMU660RA_SCL_PIN            (SOFT_IIC_SCL_D9)                           // 软件 IIC SCL 引脚 连接 IMU660RA 的 SCL 引脚
#define IMU660RA_SDA_PIN            (SOFT_IIC_SDA_E8)                           // 软件 IIC SDA 引脚 连接 IMU660RA 的 SDA 引脚
#define IMU660RA_SOFT_IIC_DELAY     ( 30 )                                      // 软件 IIC 的时钟延时周期 数值越小 IIC
               
#define IMU660RA_TIMEOUT_COUNT      (0x00FF)                                    // IMU660RA 超时计数

#define IMU660RA_DEV_ADDR           (0x69)                                      // SA0接地：0x68 SA0上拉：0x69 模块默认上拉
#define IMU660RA_SPI_W              (0x00)
#define IMU660RA_SPI_R              (0x80)

#define IMU660RA_CHIP_ID            (0x00)
#define IMU660RA_PWR_CONF           (0x7C)
#define IMU660RA_PWR_CTRL           (0x7D)
#define IMU660RA_INIT_CTRL          (0x59)
#define IMU660RA_INIT_DATA          (0x5E)
#define IMU660RA_INT_STA            (0x21)
#define IMU660RA_ACC_ADDRESS        (0x0C)
#define IMU660RA_GYRO_ADDRESS       (0x12)
#define IMU660RA_ACC_CONF           (0x40)
#define IMU660RA_ACC_RANGE          (0x41)
#define IMU660RA_GYR_CONF           (0x42)
#define IMU660RA_GYR_RANGE          (0x43)

#define IMU660RA_ACC_SAMPLE         (0x02)                                      // 加速度计量程
// 设置为:0x00 陀螺仪量程为:±2000dps     获取到的陀螺仪数据 除以 16.4       可以转化为带物理单位的数据 单位为：°/s
// 设置为:0x01 陀螺仪量程为:±1000dps     获取到的陀螺仪数据 除以 32.8       可以转化为带物理单位的数据 单位为：°/s
// 设置为:0x02 陀螺仪量程为:±500 dps     获取到的陀螺仪数据 除以 65.6       可以转化为带物理单位的数据 单位为：°/s
// 设置为:0x03 陀螺仪量程为:±250 dps     获取到的陀螺仪数据 除以 131.2      可以转化为带物理单位的数据 单位为：°/s
// 设置为:0x04 陀螺仪量程为:±125 dps     获取到的陀螺仪数据 除以 262.4      可以转化为带物理单位的数据 单位为：°/s

#define IMU660RA_GYR_SAMPLE         (0x00)                                      // 陀螺仪量程
// 设置为:0x00 加速度计量程为:±2g         获取到的加速度计数据 除以 16384   可以转化为带物理单位的数据 单位：g(m/s^2)
// 设置为:0x01 加速度计量程为:±4g         获取到的加速度计数据 除以 8192    可以转化为带物理单位的数据 单位：g(m/s^2)
// 设置为:0x02 加速度计量程为:±8g         获取到的加速度计数据 除以 4096    可以转化为带物理单位的数据 单位：g(m/s^2)
// 设置为:0x03 加速度计量程为:±16g        获取到的加速度计数据 除以 2048    可以转化为带物理单位的数据 单位：g(m/s^2)


extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;                 // 三轴陀螺仪数据      gyro (陀螺仪)
extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;                    // 三轴加速度计数据     acc (accelerometer 加速度计)


void  imu660ra_get_acc              (void);                                     // 获取 IMU660RA 加速度计数据
void  imu660ra_get_gyro             (void);                                     // 获取 IMU660RA 陀螺仪数据
float imu660ra_acc_transition       (int16 acc_value);                          // 将 IMU660RA 加速度计数据转换为实际物理数据
float imu660ra_gyro_transition      (int16 gyro_value);                         // 将 IMU660RA 陀螺仪数据转换为实际物理数据
uint8 imu660ra_init                 (void);                                     // 初始化 IMU660RA


#endif /* DEVICE_CODE_ZF_DEVICE_IMU660RA_H_ */
