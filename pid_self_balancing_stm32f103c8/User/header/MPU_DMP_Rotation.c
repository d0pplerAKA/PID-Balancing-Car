#include "MPU_DMP_Rotation.h"

float dpm_roll_offset = 0.0f;
float dpm_pitch_offset = 2.8f;
float dpm_yaw_offset = 0.0f;

uint8_t MPU_DMP_init(void)
{
    uint8_t res = 0;
    struct int_param_s int_param;

    if(mpu_init(&int_param) == 0)
    {
        res = mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);													//设置所需要的传感器
		if(res) return 1; 
		res = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);												//设置FIFO
		if(res) return 2; 
		res = mpu_set_sample_rate(13);																		//设置采样率
		if(res) return 3; 
		res = dmp_load_motion_driver_firmware();															//加载dmp固件
		if(res) return 4; 
		res = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));						//设置陀螺仪方向
		if(res) return 5; 
		res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT |DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT | 
                                DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO| DMP_FEATURE_GYRO_CAL);
		if(res) return 6; 
		res = dmp_set_fifo_rate(13);																		//设置DMP输出速率(最大不超过200Hz)
		if(res) return 7;
		/************************/
		//res = run_self_test();																				//自检
		//if(res) return 8;
		/************************/
		res = mpu_set_dmp_state(1);																			//使能DMP
		if(res) return 9;
    }

    return 0;
}

uint8_t run_self_test(void)
{
    int res;

    long gyro[3], accel[3];

    res = mpu_run_self_test(gyro, accel);
    if(res == 0x3)
    {
        float gyro_sens;
        unsigned short accel_sens;

        mpu_get_gyro_sens(&gyro_sens);
        gyro[0] = (long)(gyro[0] * gyro_sens);
		gyro[1] = (long)(gyro[1] * gyro_sens);
		gyro[2] = (long)(gyro[2] * gyro_sens);
		dmp_set_gyro_bias(gyro);

		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);

		return 0;
    }
    else    return 1;
}

uint8_t MPU_DMP_get_data(MPU_Rotation *_mpu)
{
	return dmp_get_data(&_mpu->roll, &_mpu->pitch, &_mpu->yaw);
}

uint8_t dmp_get_data(float *roll, float *pitch, float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 

	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more)) return 1;

	mpu.accel_x = (float) ((float) accel[0] / MPU_ACCEL_Divisor);
	mpu.accel_y = (float) ((float) accel[1] / MPU_ACCEL_Divisor);
	mpu.accel_z = (float) ((float) accel[2] / MPU_ACCEL_Divisor);

	mpu.gyro_x = (float) ((float) gyro[0] / MPU_GYRO_Divisor);
	mpu.gyro_y = (float) ((float) gyro[1] / MPU_GYRO_Divisor);
	mpu.gyro_z = (float) ((float) gyro[2] / MPU_GYRO_Divisor);

	if(sensors & INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;	//q30格式转换为浮点数
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//计算得到俯仰角/横滚角/航向角

		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) * MPU_RAD_TO_DEG + dpm_roll_offset;	// roll
		*pitch = arcsin_safe(-2 * q1 * q3 + 2 * q0* q2) * MPU_RAD_TO_DEG + dpm_pitch_offset;	// pitch
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * MPU_RAD_TO_DEG + dpm_yaw_offset;	//yaw
	}
	else	return 2;

	return 0;
}



