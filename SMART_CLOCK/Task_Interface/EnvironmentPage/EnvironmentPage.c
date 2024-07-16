#include "EnvironmentPage.h"
#include "menuDisp.h"
#include "voice_command.h"

#define ENVI_PAGE 1

/* Prototype function */
static void prev_Cb();
static void environment_draw();
static void environment_getVal();
static void speech_environment_cmd(uint8_t *cmd);

/* Global Variable */
static char data_buf_pressure[20], data_buf_temperatur[20], data_buf_humidity[20];

static uint8_t THIS_PAGE = 0;
static uint8_t idx_back = ENVI_PAGE+1;

void init_environment_disp(){

	button.attachPressed(&btn_obj[3],prev_Cb);

	u8g2_ClearDisplay(&u8g2_obj);
	u8g2_ClearBuffer(&u8g2_obj);

	THIS_PAGE = ENVI_PAGE;
	speech_command = 0;
}

void deinit_environment_disp(){

	button.dettachPressed(&btn_obj[3]);//BACK

	u8g2_ClearDisplay(&u8g2_obj);
	u8g2_ClearBuffer(&u8g2_obj);
}

static void environment_getVal(){
#ifdef USE_DPS310
	sprintf(data_buf_pressure  ,"Pres :%0.2f\t hPa",dps_sensor.pressure);
	sprintf(data_buf_temperatur,"Temp :%0.2f\t C\xB0",dps_sensor.temperature);
#endif

#ifdef USE_BMP280
	sprintf(data_buf_pressure  ,"Pres :%0.2f\t hPa",bmp280_sensor.pressure);
	sprintf(data_buf_temperatur,"Temp :%0.2f\t C\xB0",bmp280_sensor.temperature);
#endif

#ifdef USE_BME680
	sprintf(data_buf_pressure  ,"Pres :%0.2f\t hPa",bme680_sensor.pressure);
	sprintf(data_buf_temperatur,"Temp :%0.2f\t C\xB0",bme680_sensor.temperature);
	sprintf(data_buf_humidity,"Humy :%0.2f\t C\xB0",bme680_sensor.humidity);
#endif

#ifdef USE_DUMMY_DATA
	sprintf(data_buf_pressure  ,"Pres :%0.2f\t hPa",bme680_sensor.pressure);
	sprintf(data_buf_temperatur,"Temp :%0.2f\t C\xB0",bme680_sensor.temperature);
	sprintf(data_buf_humidity,"Humy :%0.2f\t C\xB0",bme680_sensor.humidity);
#endif
}

static void environment_draw(){
	environment_getVal();
	u8g2_DrawStr(&u8g2_obj, 0, 10, "Environment Data");

#ifdef USE_DUMMY_DATA
	u8g2_DrawStr(&u8g2_obj, 0, 30, data_buf_pressure);
	u8g2_DrawStr(&u8g2_obj, 0, 40, data_buf_temperatur);
	u8g2_DrawStr(&u8g2_obj, 0, 50, data_buf_humidity);
#endif

#ifdef USE_DPS310
	u8g2_DrawStr(&u8g2_obj, 0, 30, data_buf_pressure);
	u8g2_DrawStr(&u8g2_obj, 0, 40, data_buf_temperatur);
#endif

#ifdef USE_BMP280
	u8g2_DrawStr(&u8g2_obj, 0, 30, data_buf_pressure);
	u8g2_DrawStr(&u8g2_obj, 0, 40, data_buf_temperatur);
#endif

#ifdef USE_BME680
	u8g2_DrawStr(&u8g2_obj, 0, 30, data_buf_pressure);
	u8g2_DrawStr(&u8g2_obj, 0, 40, data_buf_temperatur);
	u8g2_DrawStr(&u8g2_obj, 0, 50, data_buf_humidity);
#endif
	send_buffer_u8g2();
}

void environment_disp(){

	init_environment_disp();

	while (1){

		speech_environment_cmd(&speech_command);
		if (THIS_PAGE == ENVI_PAGE)
			environment_draw();

		else {
			deinit_environment_disp();
			menu_disp();
		}
		vTaskDelay(20);
	}
	deinit_environment_disp();
}

static void prev_Cb(){

	THIS_PAGE = idx_back; //index_back
}

static void speech_environment_cmd(uint8_t *cmd){

	switch(*cmd){
	case BACK_CMD:
		THIS_PAGE = idx_back; //index_back
		break;
	}
}
