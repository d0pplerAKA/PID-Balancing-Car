#include "MPU_Rotation.h"

float pitch_offset_kalman = -1;
float pitch_offset_mahony = -0.35;
float roll_offset_mahony = -2.475;

/**Mahony**/
volatile float normalize;
volatile float ax, ay, az, gx, gy, gz;
volatile float vx, vy, vz;
volatile float ex, ey, ez;
volatile float temp_q0, temp_q1, temp_q2;

volatile float M_q0 = 1.0f, M_q1 = 0.0f, M_q2 = 0.0f, M_q3 = 0.0f;   //初始四元数

volatile float M_dt = 1.0f / MPU_Sample_Freq;    //时间积分

volatile float ki = 0.001;	//控制数据准确度 减小误差值 [积分调节系数]
volatile float kp = 0.2825;	//控制系统稳定度 减少波动量 [比例调节系数]

volatile float exInt = 0, eyInt = 0, ezInt = 0;
/**********/

/**Kalman**/
volatile float K_dt = 1.0f / MPU_Sample_Freq;

volatile float Q_angle_X = 0.0001;
volatile float Q_gyro_X = 0.3;
volatile float R_angle_X = 0.5;
volatile char C_0_X = 1;

volatile float Q_bias_X, Angle_err_X, PCt_0_X, PCt_1_X, E_X, K_0_X, K_1_X, t_0_X, t_1_X;
volatile float P_X[4] = {0, 0, 0, 0};
volatile float PP_X[2][2] = {{1, 0}, {0, 1}};
volatile float K_angle_X;

volatile float Q_angle_Y = 0.0001;
volatile float Q_gyro_Y = 0.3;
volatile float R_angle_Y = 0.5;
volatile char C_0_Y = 1;

volatile float Q_bias_Y, Angle_err_Y, PCt_0_Y, PCt_1_Y, E_Y, K_0_Y, K_1_Y, t_0_Y, t_1_Y;
volatile float P_Y[4] = {0, 0, 0, 0};
volatile float PP_Y[2][2] = {{1, 0}, {0, 1}};
volatile float K_angle_Y;

/**********/

/*****complementary*****/
//volatile float C_dt = 1 / MPU_Sample_Freq;
volatile float C_dt = 0.5;
volatile float C_angle;
volatile float C_K = 1;

/***********************/

float arcsin_safe(float x)
{
    if(isnan(x))
        return 0.0f;
    else if(x >= 1.0f)
        return MPU_PI / 2;
    else if(x <= -1.0f)
        return -MPU_PI / 2;

    return asin(x);
}

float invSqrt(float x)
{
    float xhalf = 0.5f * x;

    long int i = *(long int *) & x;              // get bits for floating value

    i =  0x5f375a86 - (i >> 1);       // gives initial guess

    x = *(float *) & i;                // convert bits back to float

    x = x * (1.5f - xhalf * x * x);     // Newton step

    return x;
}

void MPU_MahonyFilter(MPU6050_Raw_Data *mpu, MPU_Rotation *mpu_r)
{
    ax = mpu->accel_x;
    ay = mpu->accel_y;
    az = mpu->accel_z;
    gx = mpu->gyro_x * MPU_DEG_TO_RAD;
    gy = mpu->gyro_y * MPU_DEG_TO_RAD;
    gz = mpu->gyro_z * MPU_DEG_TO_RAD;
    //装载数据

    /*
    if(ax * ay * az == 0)
    {
        //println("Exit");
        return;
    }
    */
    normalize = invSqrt((ax * ax + ay * ay + az * az));
    ax = ax * normalize;
    ay = ay * normalize;
    az = az * normalize;

    //数据归一化，统一度量衡    (scaling)

    vx = 2 * (M_q1 * M_q3 - M_q0 * M_q2);
    vy = 2 * (M_q0 * M_q1 + M_q2 * M_q3);
    vz = M_q0 * M_q0 - M_q1 * M_q1 - M_q2 + M_q2 + M_q3 * M_q3;
    //四元数计算重力分量

    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);
    //作差积 取得误差    加速度计重力分量 和 四元数重力分量

    exInt = exInt + ex * ki;
    eyInt = eyInt + ey * ki;
    ezInt = ezInt + ez * ki;
    //对误差积分得到单位时间内得累计误差

    gx += exInt + kp * ex;
    gy += eyInt + kp * ey;
    gz += ezInt + kp * ez;
    //反馈积分至角速度
    //比例运算  对误差采样
    //采样程度决定系统的稳定程度

    M_q0 += (- M_q1 * gx - M_q2 * gy - M_q3 * gz) * M_dt / 2;
    M_q1 += (M_q0 * gx + M_q2 * gz - M_q3 * gy) * M_dt / 2;
    M_q2 += (M_q0 * gy - M_q1 * gz + M_q3 * gx) * M_dt / 2;
    M_q3 += (M_q0 * gz + M_q1 * gy - M_q2 * gx) * M_dt / 2;
    //更新四元数

    normalize = invSqrt((M_q0 * M_q0 + M_q1 * M_q1 + M_q2 * M_q2 + M_q3 * M_q3));

    M_q0 = M_q0 * normalize;
    M_q1 = M_q1 * normalize;
    M_q2 = M_q2 * normalize;
    M_q3 = M_q3 * normalize;
    //四元数归一化

    mpu_r->pitch    =    -arcsin_safe(-2 * M_q1 * M_q3 + 2 * M_q0 * M_q2) * MPU_RAD_TO_DEG + pitch_offset_mahony;
    mpu_r->roll     =    atan2(2 * M_q2 * M_q3 + 2 * M_q0 * M_q1, -2 * M_q1 * M_q1 - 2 * M_q2 *M_q2 + 1) * MPU_RAD_TO_DEG + roll_offset_mahony;
    mpu_r->yaw      =    atan2(2 * (M_q1 * M_q2 + M_q0 * M_q3), M_q0 * M_q0 + M_q1 * M_q1 - M_q2 * M_q2 - M_q3 * M_q3) * MPU_RAD_TO_DEG;
    //填值
}

float MPU_KalmanFilter_temp_X(float Angle, float Gyro)
{
    K_angle_X += (Gyro - Q_bias_X) * K_dt;

    P_X[0] = Q_angle_X - PP_X[0][1] - PP_X[1][0];
    P_X[1] = -PP_X[1][1];
    P_X[2] = -PP_X[1][1];
    P_X[3] = Q_gyro_X;

    PP_X[0][0] += P_X[0] * K_dt;
	PP_X[0][1] += P_X[1] * K_dt;
	PP_X[1][0] += P_X[2] * K_dt;
	PP_X[1][1] += P_X[3] * K_dt;

	PCt_0_X = C_0_X * PP_X[0][0];
    PCt_1_X = C_0_X * PP_X[1][0];

    E_X = R_angle_X + C_0_X * PCt_0_X;

    K_0_X = PCt_0_X / E_X;
    K_1_X = PCt_1_X / E_X;

    t_0_X = PCt_0_X;
    t_0_X = C_0_X * PP_X[0][1];
    PP_X[0][0] -= K_0_X * t_0_X;
    PP_X[0][1] -= K_0_X * t_1_X;
    PP_X[1][0] -= K_1_X * t_0_X;
    PP_X[1][1] -= K_1_X * t_1_X;

    Angle_err_X = Angle - K_angle_X;
    K_angle_X += K_0_X * Angle_err_X;
    Q_bias_X += K_1_X * Angle_err_X;    

    return K_angle_X;
}

float MPU_KalmanFilter_temp_Y(float Angle, float Gyro)
{
    K_angle_Y += (Gyro - Q_bias_Y) * K_dt;

    P_Y[0] = Q_angle_Y - PP_Y[0][1] - PP_Y[1][0];
    P_Y[1] = -PP_Y[1][1];
    P_Y[2] = -PP_Y[1][1];
    P_Y[3] = Q_gyro_Y;


    PP_Y[0][0] += P_Y[0] * K_dt;
	PP_Y[0][1] += P_Y[1] * K_dt;
	PP_Y[1][0] += P_Y[2] * K_dt;
	PP_Y[1][1] += P_Y[3] * K_dt;


	PCt_0_Y = C_0_Y * PP_Y[0][0];
    PCt_1_Y = C_0_Y * PP_Y[1][0];

    E_Y = R_angle_Y + C_0_Y * PCt_0_Y;

    K_0_Y = PCt_0_Y / E_Y;
    K_1_Y = PCt_1_Y / E_Y;

    t_0_Y = PCt_0_Y;
    t_0_Y = C_0_Y * PP_Y[0][1];
    PP_Y[0][0] -= K_0_Y * t_0_Y;
    PP_Y[0][1] -= K_0_Y * t_1_Y;
    PP_Y[1][0] -= K_1_Y * t_0_Y;
    PP_Y[1][1] -= K_1_Y * t_1_Y;

    Angle_err_Y = Angle - K_angle_Y;
    K_angle_Y += K_0_Y * Angle_err_Y;
    Q_bias_Y += K_1_Y * Angle_err_Y;

    return K_angle_Y;
}

void MPU_KalmenFilter(MPU_Rotation *mpu_r)
{
    mpu_r->roll = MPU_KalmanFilter_temp_X(MPU_Accel_Roll(&mpu), MPU_GYRO_X(&mpu)) * MPU_RAD_TO_DEG;
    mpu_r->pitch = MPU_KalmanFilter_temp_Y(MPU_Accel_Pitch(&mpu), MPU_GYRO_Y(&mpu)) * MPU_RAD_TO_DEG + pitch_offset_kalman;
    //mpu_r->pitch /= 0.56;
}

float MPU_Accel_Pitch(MPU6050_Raw_Data *mpu)
{
    float temp = (atan2(mpu->accel_x , sqrt(mpu->accel_y * mpu->accel_y + mpu->accel_z * mpu->accel_z)));

    return temp;
}

float MPU_Accel_Roll(MPU6050_Raw_Data *mpu)
{
    float temp = (atan2(mpu->accel_y , sqrt(mpu->accel_x * mpu->accel_x + mpu->accel_z * mpu->accel_z)));

    return temp;
}

float MPU_GYRO_X(MPU6050_Raw_Data *mpu)
{
    float temp = mpu->gyro_x;

    return temp;
}

float MPU_GYRO_Y(MPU6050_Raw_Data *mpu)
{
    float temp = mpu->gyro_y;

    return temp;
}

void MPU_Print_Standard_Data(MPU_Rotation *mpu_r)
{
    println("Standard Data: ");
    printf("Current pitch: %.2f\n", mpu_r->pitch);
    printf("Current roll: %.2f\n", mpu_r->roll);
    printf("Current yaw: %.2f\n", mpu_r->yaw);
    println("");
}
