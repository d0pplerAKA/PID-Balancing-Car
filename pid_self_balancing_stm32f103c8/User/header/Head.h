#ifndef __HEAD_H__
#define __HEAD_H__


#include "UART.h"
#include "OLED_SH1106.h"

#define UART                                0
#define OLED                                1
#define DEBUG_ALL                           -1

#define MPU_PI                              3.1415926535f
#define MPU_G                               9.806f

#define MPU_RAD_TO_DEG                      57.295779513082f // 弧度转角度的转换率
#define MPU_DEG_TO_RAD                      0.0174532925193f // 角度转弧度的转换率

#define MPU_ACCEL_Divisor                   16384
#define MPU_GYRO_Divisor                    16.384f


/*MPU6050 Config*/

/*
VCC -> 3.3v

SCL -> PB6
SDA -> PB7

AD0 -> PA15 / GND
*/

/*<R/W> 采样率*/
#define MPU_Sample_Rate_Divider             0x19
#define MPU_Sample_Rate_Divider_Set         0x0F

/*<R/W> 滤波设置（和外部帧同步）*/
#define MPU_CONFIG                          0x1A
#define MPU_CONFIG_Set                      0x01


/*<R/W> 陀螺仪范围设置（和是否自检）
 Bit7   Bit6   Bit5  Bit4 Bit3  Bit2  Bit1  Bit0
[XGST] [YGST] [ZGST] [FS_SEL]  [       --       ]

 FS_SEL   Full Scale Range
   0        +- 250 °/s                      
   1        +- 500 °/s                      
   2        +- 1000 °/s                     
   3        +- 2000 °/s                     √   

Current setting: 0x18   |   0001 1000
*/
#define MPU_Gyro_Config                     0x1B
#define MPU_Gyro_Config_Set                 0x18


/*<R/W> 加速度精度设置（和是否自检）
 Bit7   Bit6   Bit5  Bit4 Bit3  Bit2  Bit1  Bit0
[XAST] [YAST] [ZAST] [AFS_SEL] [       --       ]

 AFS_SEL   Full Scale Range
    0           +- 2g                       √
    1           +- 4g                       
    2           +- 8g                       
    3           +- 16g

Current setting: 0x00   |   0000 0000
*/
#define MPU_Accel_Config                    0x1C
#define MPU_Accel_Config_Set                0x00


/*<R/W> 数据流FIFO*/
#define MPU_FIFO_EN                         0x23
#define MPU_FIFO_EN_Set                     0x00

/*<R/W> 通信频率设置值（和多重主机、数据准备中断，从机3数据流FIFIO和通信截止方式设置）*/
#define MPU_I2C_MST_Ctrl                    0x24
#define MPU_I2C_MST_Ctrl_Set                0x09


/*<R/W> 中断使能*/
#define MPU_INT_ENABLE                      0x38
#define MPU_INT_ENABLE_Set                  0x00

/*<R> 轴加速度*/
#define MPU_ACCEL_X_H                       0x3B
#define MPU_ACCEL_X_L                       0x3C
#define MPU_ACCEL_Y_H                       0x3D
#define MPU_ACCEL_Y_L                       0x3E
#define MPU_ACCEL_Z_H                       0x3F
#define MPU_ACCEL_Z_L                       0x40

/*<R> 温度*/
#define MPU_Temp_H                          0x41
#define MPU_Temp_L                          0x42

/*<R> 陀螺仪*/
#define MPU_GYRO_X_H                        0x43
#define MPU_GYRO_X_L                        0x44
#define MPU_GYRO_Y_H                        0x45
#define MPU_GYRO_Y_L                        0x46
#define MPU_GYRO_Z_H                        0x47
#define MPU_GYRO_Z_L                        0x48

/*<W> 重置传感器信号路径*/
#define MPU_Signal_Path_Reset               0x68
#define MPU_Signal_Path_Reset_Set           0x07

/*<R/W> 重置FIFO，主机分配权，信号路径重置*/
#define MPU_User_Ctrl                       0x6A
#define MPU_User_Ctrl_Set                   0x00

/*<R/W> 电源管理 1(睡眠模式、唤醒设置，温度传感器启用/禁用)*/
#define MPU_PWR_MGMT_1                      0x6B
#define MPU_PWR_MGMT_1_Set                  0x05

#define MPU_PWR_MGMT_2                      0x6C
#define MPU_PWR_MGMT_2_Set                  0x00


/*<R> 身份值寄存器*/
/*
应输出 0x68!!!  AD0 -> GND
应输出 0x69!!!  AD0 -> VCC
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
 -   [         who am i         ]    -
*/
#define MPU_WHO_AM_I                        0x75

/*MPU6050 Config*/

typedef struct
{
    float temperature;

    float accel_x;
    float accel_y;
    float accel_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

}   MPU6050_Raw_Data;

typedef struct
{
	float pitch;
	float roll;
    float yaw;

}	MPU_Rotation;

extern MPU6050_Raw_Data mpu;
extern MPU_Rotation mpu_rotation;

extern int DEBUG_OUTPUT;


void DEBUG_UART(unsigned char data);
void DEBUG_OLED(unsigned char data);

void Sys_Reset(void);

#endif
