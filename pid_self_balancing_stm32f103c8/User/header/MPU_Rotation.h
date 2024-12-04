#ifndef _MPU_ROTATION_H__
#define _MPU_ROTATION_H__

#include "stm32f10x.h"
#include "math.h"
#include "UART.h"
#include "Head.h"

#define MPU_Sample_Freq             133.3333f

#define MPU_PITCH                   0
#define MPU_ROLL                    1

float arcsin_safe(float x);
float invSqrt(float x);

void MPU_MahonyFilter(MPU6050_Raw_Data *mpu, MPU_Rotation *mpu_r);

float MPU_KalmanFilter_temp_X(float Angle, float Gyro);
float MPU_KalmanFilter_temp_Y(float Angle, float Gyro);

void MPU_KalmenFilter(MPU_Rotation *mpu_r);

float MPU_Accel_Roll(MPU6050_Raw_Data *mpu);
float MPU_Accel_Pitch(MPU6050_Raw_Data *mpu);
float MPU_GYRO_X(MPU6050_Raw_Data *mpu);
float MPU_GYRO_Y(MPU6050_Raw_Data *mpu);

void MPU_Print_Standard_Data(MPU_Rotation *mpu_r);

#endif
