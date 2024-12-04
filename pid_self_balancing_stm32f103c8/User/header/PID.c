#include "PID.h"

PID_Struct PID_Struct_degree;
PID_Struct PID_Struct_motor;

volatile float pid_range = 0.011;
float pid_orientation = 0;

/*
P 比例
I 增幅
D 预测
*/

//Degree
volatile float Kp_d = 247.5;
//volatile float Ki_d = 0.00;
volatile float Kd_d = 18.05;

//Motor
volatile float Kp_m = -17.5;
volatile float Ki_m;
//volatile float Kd_m = 0.00;

volatile float Kp_o = -0.8;

void Holy_fuck(void)
{
    LED_OFF;

    MPU_DMP_get_data(&mpu_rotation);
    //MPU_Refresh_Basic_Data(&mpu);
    //MPU_KalmenFilter(&mpu_rotation);
    //MPU_MahonyFilter(&mpu, &mpu_rotation);
    //NRF_Receiver_Read(&nrf_data);
    
    if(mpu_rotation.pitch < 0)
        Motor_Set_DIR(MOTOR_DIR_R);
    else
        Motor_Set_DIR(MOTOR_DIR_L);

    PID_Generate_PWM();
    Motor_Set_PWM(PID_output_pwm - pid_orientation, PID_output_pwm + pid_orientation);
}

void PID_Init(void)
{
    PID_Clear(&PID_Struct_degree);
    PID_Clear(&PID_Struct_motor);

    PID_Struct_degree.val_target = PID_Median;
}

void PID_Clear(PID_Struct *pid)
{
    pid->val_proportional = 0;
    pid->val_integral = 0;
    pid->val_derivative = 0;

    pid->val_err = 0;
    pid->val_err_previous = 0;

    pid->val_output = 0;
}


void PID_Control_Degree(PID_Struct *pid, MPU_Rotation *mpu_r, MPU6050_Raw_Data *mpu_raw, float val_target, float *val_output)
{
    /*
    float _pitch = mpu_r->pitch;

    if(_pitch < 0)
        Motor_Set_DIR(MOTOR_DIR_R);
    else
        Motor_Set_DIR(MOTOR_DIR_L);

    if(_pitch <= pid_range && _pitch >= pid_range * (-1))
    {
        PID_Clear(&PID_Struct_degree);
        PID_Clear(&PID_Struct_motor);

        *val_output = 0;

        return -1;
    }
    else
    {
        
    }
    if(_pitch > 0)
        _pitch *= -1.0f;

    pid->val_err = pid->val_target - _pitch;
    pid->val_proportional = pid->val_err * Kp_d;

    pid->val_integral += pid->val_err * Ki_d * 0.5;

    if(pid->val_integral > PID_DEGREE_I_MAX)
        pid->val_integral = PID_DEGREE_I_MAX;
    if(pid->val_integral < PID_DEGREE_I_MIN)
        pid->val_integral = PID_DEGREE_I_MIN;

    pid->val_derivative = (pid->val_err - pid->val_err_previous) * Kd_d / 0.5;

    pid->val_err_previous = pid->val_err;

    pid->val_output = pid->val_proportional + pid->val_integral + pid->val_derivative;

    *val_output = pid->val_output;
    */
    /*
    if(mpu_r->pitch <= pid_range && mpu_r->pitch >= pid_range * -1.0f)
    {
        PID_Clear(&PID_Struct_degree);
        PID_Clear(&PID_Struct_motor);

        PID_output_pwm = 0;

        return;
    }
    */
    if(mpu_r->pitch < pid_range && mpu_r->pitch > pid_range * -1.0f)
    {
        PID_Clear(&PID_Struct_degree);
        PID_Clear(&PID_Struct_motor);

        *val_output = 0;

        return;
    }

    pid->val_proportional = (mpu_r->pitch - val_target) * Kp_d;
    pid->val_derivative = mpu_raw->gyro_y * Kd_d;

    pid->val_output = pid->val_proportional + pid->val_derivative;

    *val_output = pid->val_output;
}

void PID_Control_Motor(PID_Struct *pid, float target_MotorSpeed, int actual_MotorSpeed, float *val_output)
{
    if(mpu_rotation.pitch < pid_range && mpu_rotation.pitch > pid_range * -1.0f)
    {
        PID_Clear(&PID_Struct_degree);
        PID_Clear(&PID_Struct_motor);

        *val_output = 0;

        return;
    }

    pid->val_err = actual_MotorSpeed * 0.3 + pid->val_err_previous * 0.7;
    pid->val_err_previous = pid->val_err;

    pid->val_integral += pid->val_err;

    if(pid->val_integral > PID_MOTOR_I_MAX)
        pid->val_integral = PID_MOTOR_I_MAX;
    if(pid->val_integral <= PID_MOTOR_I_MIN)
        pid->val_integral = PID_MOTOR_I_MIN;
    
    Ki_m = Kp_m / 200.0f;
    pid->val_output = pid->val_err * Kp_m + pid->val_integral * Ki_m;

    *val_output = pid->val_output;
    /*
    pid->val_target = target_MotorSpeed;

    pid->val_err = pid->val_target - actual_MotorSpeed;
    pid->val_proportional = pid->val_err * Kp_m;

    pid->val_integral += pid->val_err * Ki_m * 0.5;

    if(pid->val_integral > PID_MOTOR_I_MAX)
        pid->val_integral = PID_MOTOR_I_MAX;
    if(pid->val_integral < PID_MOTOR_I_MIN)
        pid->val_integral = PID_MOTOR_I_MIN;

    pid->val_derivative = (pid->val_err - pid->val_err_previous) * Kd_m / 0.5;

    pid->val_err_previous = pid->val_err;

    pid->val_output = pid->val_proportional + pid->val_integral + pid->val_derivative;

    *val_output = pid->val_output;
    */
}

void PID_Control_Orientation(MPU6050_Raw_Data *mpu_raw, float *val_output)
{
    if(mpu_rotation.pitch < pid_range && mpu_rotation.pitch > pid_range * -1.0f)
    {
        PID_Clear(&PID_Struct_degree);
        PID_Clear(&PID_Struct_motor);

        *val_output = 0;

        return;
    }

    *val_output = Kp_o * mpu_raw->gyro_z;
}

void PID_Generate_PWM(void)
{   
    float pid_degree = 0, pid_motor = 0, pid_output = 0;

    PID_Control_Motor(&PID_Struct_motor, 0, motor_currentSpeed, &pid_motor);        //  速度环  PI
    PID_Control_Degree(&PID_Struct_degree, &mpu_rotation, &mpu, 0, &pid_degree);    //  直立环  PD
    PID_Control_Orientation(&mpu, &pid_orientation);

    /*
    if(PID_Control_Degree(&PID_Struct_degree, &mpu_rotation, &pid_degree) != 0)
    {
        PID_output_pwm = 0;
        return;
    }

    //PID_Control_Motor(&PID_Struct_motor, pid_degree, motor_currentSpeed, &pid_motor);

    pid_output = pid_degree;

    if(pid_output > 10000.0f)
        pid_output = 10000;
    if(pid_output < 0)
        pid_output = 0;
    */

    pid_output = pid_degree + pid_motor;

    PID_output_pwm = pid_output;
}
