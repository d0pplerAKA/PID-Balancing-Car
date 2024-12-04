#ifndef __MPU6050_H__
#define __MPU6050_H__


#include "math.h"
#include "I2c.h"
#include "UART.h"
#include "Head.h"
#include "Led.h"

#define MPU_ACCEL                           1
#define MPU_GYRO                            2
#define MPU_TEMP                            0


void MPU_Init(void);

void MPU_Get_Self_Address(unsigned char *ptr);

void MPU_Get_Temp(float *ptr);

void MPU_Get_Accel_X(float *ptr);
void MPU_Get_Accel_Y(float *ptr);
void MPU_Get_Accel_Z(float *ptr);

void MPU_Get_Gyro_X(float *ptr);
void MPU_Get_Gyro_Y(float *ptr);
void MPU_Get_Gyro_Z(float *ptr);

void MPU_Refresh_Basic_Data(MPU6050_Raw_Data *mpu);

void MPU_Print_Basic_Data(MPU6050_Raw_Data *mpu);

void fast_temp(void);

float MPU_Bit_Combine(unsigned char H_Bits, unsigned char L_Bits, int type);


#endif
