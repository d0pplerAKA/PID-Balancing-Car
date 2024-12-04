#ifndef __MPU_DMP_ROTATION_H__
#define __MPU_DMP_ROTATION_H__

#include "eMPL_outputs.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"
#include "log.h"
#include "mltypes.h"
#include "mpl.h"
#include "mpu.h"
#include "packet.h"

#include "Head.h"
#include "MPU_Rotation.h"

#define q30 1073741824.0f


static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1 };

uint8_t MPU_DMP_init(void);
uint8_t run_self_test(void);
uint8_t MPU_DMP_get_data(MPU_Rotation *_mpu);
uint8_t dmp_get_data(float *roll, float *pitch, float *yaw);

#endif
