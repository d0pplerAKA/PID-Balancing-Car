#include "MPU6050.h"


float offset_accel_x = 0, offset_accel_y = 0.1, offset_accel_z = 0.055;
float offset_gyro_x = 4.07, offset_gyro_y = 1.14, offset_gyro_z = 1.77;

unsigned int I2C_LOG = DISABLE;

void MPU_Init(void)
{
    DEBUG_OUTPUT = UART;

    unsigned char status[11];
    unsigned char flag = 1;
    
    while(flag == 1)
    {
        LED_ON;

        for(uint8_t i = 0; i < 11; i++)
            status[i] = I2C_SUCCESS;

        status[0] = I2C_Write_MPU(MPU_PWR_MGMT_1, 0x80);
        delay(200);
        status[0] = I2C_Write_MPU(MPU_PWR_MGMT_1, 0x80);
        delay(200);
        status[1] = I2C_Write_MPU(MPU_PWR_MGMT_1, MPU_PWR_MGMT_1_Set);
        status[2] = I2C_Write_MPU(MPU_PWR_MGMT_2, MPU_PWR_MGMT_2_Set);
        status[3] = I2C_Write_MPU(MPU_Sample_Rate_Divider, MPU_Sample_Rate_Divider_Set);
        status[4] = I2C_Write_MPU(MPU_CONFIG, MPU_CONFIG_Set);
        status[5] = I2C_Write_MPU(MPU_Gyro_Config, MPU_Gyro_Config_Set);
        status[6] = I2C_Write_MPU(MPU_Accel_Config, MPU_Accel_Config_Set);
        status[7] = I2C_Write_MPU(MPU_FIFO_EN, MPU_FIFO_EN_Set);
        status[8] = I2C_Write_MPU(MPU_I2C_MST_Ctrl, MPU_I2C_MST_Ctrl_Set);
        status[9] = I2C_Write_MPU(MPU_INT_ENABLE, MPU_INT_ENABLE_Set);
        status[10] = I2C_Write_MPU(MPU_User_Ctrl, MPU_User_Ctrl_Set);

        for(uint8_t i = 0; i < 11; i++)
        {
            if(status[i] != I2C_SUCCESS)
            {
                flag = 1;
                printf("MPU might initialized wrong! ERROR Code: %d, finds at %d\n", status[i], i);
            }
            else flag = 0;
        }
        
        LED_OFF;
        //printf("\n");
    }

//    printf("MPU Init success!\n");
}


void MPU_Get_Self_Address(unsigned char *ptr)
{
    unsigned char error = I2C_Read_MPU(MPU_WHO_AM_I, ptr);
    
    if(I2C_LOG)
    {
        if(error != I2C_SUCCESS)
            printf("Cannot get Self Address! ERROR finds at: %d\n", error);
    }
}

void MPU_Get_Temp(float *ptr)
{
    unsigned char addr1, addr2;
    unsigned int error1, error2;

    error1 = I2C_Read_MPU(MPU_Temp_H, &addr1);
    error2 = I2C_Read_MPU(MPU_Temp_L, &addr2);

    if(I2C_LOG)
    {
        if(error1 != I2C_SUCCESS)
            printf("Cannot get Temp High Bits! ERROR finds at %d\n", error1);

        if(error2 != I2C_SUCCESS)
            printf("Cannot get Temp LOW Bits! ERROR finds at %d\n", error2);
    }


    *ptr = MPU_Bit_Combine(addr1, addr2, MPU_TEMP);
}

void MPU_Get_Accel_X(float *ptr)
{
    unsigned char addr1, addr2;
    unsigned int error1, error2;

    error1 = I2C_Read_MPU(MPU_ACCEL_X_H, &addr1);
    error2 = I2C_Read_MPU(MPU_ACCEL_X_L, &addr2);

    if(I2C_LOG)
    {
        if(error1 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z High Bits! ERROR finds at %d\n", error1);

        if(error2 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z LOW Bits! ERROR finds at %d\n", error2);
    }

    *ptr = (MPU_Bit_Combine(addr1, addr2, MPU_ACCEL) + offset_accel_x) * MPU_G;
}

void MPU_Get_Accel_Y(float *ptr)
{
    unsigned char addr1, addr2;
    unsigned int error1, error2;

    error1 = I2C_Read_MPU(MPU_ACCEL_Y_H, &addr1);
    error2 = I2C_Read_MPU(MPU_ACCEL_Y_L, &addr2);


    if(I2C_LOG)
    {
        if(error1 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z High Bits! ERROR finds at %d\n", error1);

        if(error2 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z LOW Bits! ERROR finds at %d\n", error2);
    }

    *ptr = (MPU_Bit_Combine(addr1, addr2, MPU_ACCEL) + offset_accel_y) * MPU_G;
}

void MPU_Get_Accel_Z(float *ptr)
{
    unsigned char addr1, addr2;
    unsigned int error1, error2;

    error1 = I2C_Read_MPU(MPU_ACCEL_Z_H, &addr1);
    error2 = I2C_Read_MPU(MPU_ACCEL_Z_L, &addr2);

    if(I2C_LOG)
    {
        if(error1 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z High Bits! ERROR finds at %d\n", error1);

        if(error2 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z LOW Bits! ERROR finds at %d\n", error2);
    }

    *ptr = (MPU_Bit_Combine(addr1, addr2, MPU_ACCEL) + offset_accel_z) * MPU_G;
}

void MPU_Get_Gyro_X(float *ptr)
{    
    unsigned char addr1, addr2;
    unsigned int error1, error2;

    error1 = I2C_Read_MPU(MPU_GYRO_X_H, &addr1);
    error2 = I2C_Read_MPU(MPU_GYRO_X_L, &addr2);

    if(I2C_LOG)
    {
        if(error1 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z High Bits! ERROR finds at %d\n", error1);

        if(error2 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z LOW Bits! ERROR finds at %d\n", error2);
    } 

    *ptr = MPU_Bit_Combine(addr1, addr2, MPU_GYRO) + offset_gyro_x;
}

void MPU_Get_Gyro_Y(float *ptr)
{
    unsigned char addr1, addr2;
    unsigned int error1, error2;

    error1 = I2C_Read_MPU(MPU_GYRO_Y_H, &addr1);
    error2 = I2C_Read_MPU(MPU_GYRO_Y_L, &addr2);

    if(I2C_LOG)
    {
        if(error1 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z High Bits! ERROR finds at %d\n", error1);

        if(error2 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z LOW Bits! ERROR finds at %d\n", error2);
    }

    *ptr = MPU_Bit_Combine(addr1, addr2, MPU_GYRO) + offset_gyro_y;
}

void MPU_Get_Gyro_Z(float *ptr)
{
    unsigned char addr1, addr2;
    unsigned int error1, error2;

    error1 = I2C_Read_MPU(MPU_GYRO_Z_H, &addr1);
    error2 = I2C_Read_MPU(MPU_GYRO_Z_L, &addr2);

    if(I2C_LOG)
    {
        if(error1 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z High Bits! ERROR finds at %d\n", error1);

        if(error2 != I2C_SUCCESS)
            printf("Cannot get Gyro_Z LOW Bits! ERROR finds at %d\n", error2);
    }

    *ptr = MPU_Bit_Combine(addr1, addr2, MPU_GYRO) + offset_gyro_z;
}

void MPU_Refresh_Basic_Data(MPU6050_Raw_Data *mpu)
{
    //MPU_Get_Temp(&mpu->temperature);

    MPU_Get_Accel_X(&mpu->accel_x);
    MPU_Get_Accel_Y(&mpu->accel_y);
    MPU_Get_Accel_Z(&mpu->accel_z);

    MPU_Get_Gyro_X(&mpu->gyro_x);
    MPU_Get_Gyro_Y(&mpu->gyro_y);
    MPU_Get_Gyro_Z(&mpu->gyro_z);
}

void MPU_Print_Basic_Data(MPU6050_Raw_Data *mpu)
{
    println("Basic Data: ");
    printf("Current temperature:    %.2f°C\n", mpu->temperature);
    printf("Acceleration:        x: %.2fm/s²  y: %.2fm/s²  z: %.2fm/s²\n", mpu->accel_x, mpu->accel_y, mpu->accel_z);
    printf("Gyro:                x: %.2frad/s y: %.2frad/s z: %.2frad/s\n", mpu->gyro_x, mpu->gyro_y, mpu->gyro_z);
    println("");
}

void fast_temp(void)
{
    float temp;
    MPU_Get_Temp(&temp);
    printf("%.3f\n", temp);
}

float MPU_Bit_Combine(unsigned char H_Bits, unsigned char L_Bits, int type)
{
    int8_t i;
    unsigned int p;
    short int p_st;
    float ans;

    for(i = 7; i >= 0; i--)
    {
        p = p << 1;
        p = p | ((H_Bits >> i) & 1);
    }

    for(i = 7; i >= 0; i--)
    {
        p = p << 1;
        p = p | ((L_Bits >> i) & 1);
    }

    p_st = (short int) p;

    if(type == MPU_ACCEL)
    {   
        ans = (float) p_st / MPU_ACCEL_Divisor;
    }
    else if(type == MPU_GYRO)
    {
        ans = (float) p_st / MPU_GYRO_Divisor;
    }
    else
    {
        ans = 36.53 + (float) p_st / (float) 340;
    }

    return ans;
}
