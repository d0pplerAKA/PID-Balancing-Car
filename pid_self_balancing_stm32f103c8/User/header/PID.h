#ifndef __PID_H__
#define __PID_H__

#include "stm32f10x.h"

#include "Head.h"
#include "EncoderMotor.h"
#include "MPU6050.h"
#include "MPU_Rotation.h"
#include "Receiver.h"
#include "Led.h"

#include "MPU_DMP_Rotation.h"

#define PID_DEGREE_I_MAX                3000
#define PID_DEGREE_I_MIN                0

#define PID_MOTOR_I_MAX                 10000
#define PID_MOTOR_I_MIN                 -10000


typedef struct
{
    /* data */
    float val_target;       //目标值
    float val_proportional;
    float val_integral;
    float val_derivative;

    float val_err;
    float val_err_previous;

    float val_output;
}PID_Struct;


extern PID_Struct PID_Struct_degree;
extern PID_Struct PID_Struct_motor;


void Holy_fuck(void);

void PID_Clear(PID_Struct *pid);
void PID_Control_Degree(PID_Struct *pid, MPU_Rotation *mpu_r, MPU6050_Raw_Data *mpu_raw, float val_target, float *val_output);
void PID_Control_Motor(PID_Struct *pid, float target_MotorSpeed, int actual_MotorSpeed, float *val_output);
void PID_Control_Orientation(MPU6050_Raw_Data *mpu_raw, float *val_output);

void PID_Generate_PWM(void);


#endif
