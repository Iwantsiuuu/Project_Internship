/*
 * task_sensor.h
 *
 *  Created on: 16 Feb 2024
 *      Author: DwiSetyabudi
 *      Edited: RidwanBahari (21 Mei 2024)
 */
#pragma once

#ifndef TASK_SENSOR_TASK_SENSOR_H_
#define TASK_SENSOR_TASK_SENSOR_H_

#include "BMP280.h"

typedef struct{
	float temperature,
		  pressure,
		  humidity,
		  gas;
}sensor_data;

extern sensor_data dps_sensor;
extern sensor_data bmp_sensor;
extern sensor_data bme680_sensor;

void sensor_App();
cy_rslt_t sensorInit();

#endif /* TASK_SENSOR_TASK_SENSOR_H_ */
