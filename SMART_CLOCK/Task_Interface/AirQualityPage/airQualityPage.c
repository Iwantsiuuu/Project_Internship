#include "airQualityPage.h"
#include "menuDisp.h"
#include "voice_command.h"
#include "stdio.h"
#include "string.h"

/*******************************************************************************
 * Macros
 *******************************************************************************/
#define ENVI_PAGE 1

/*******************************************************************************
 * Prototipe function
 *******************************************************************************/
static void prev_Cb();
static void airQuality_draw();
static void airQuality_getVal();
static void init_airQuality_disp();
static void deinit_airQuality_disp();
static void speech_airquality_cmd(uint8_t *cmd);

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
#ifdef USE_BMP280
static char data_buf_pressure[20];
static char data_buf_temperatur[20];
#endif

#ifdef USE_DPS310
static char data_buf_pressure[20];
static char data_buf_temperatur[20];
#endif

#ifdef USE_BME680
static char data_buf_gas[20];
#endif

#ifdef USE_DUMMY_DATA
static char data_buf_gas[20];
#endif

static uint8_t THIS_PAGE = 0;
static uint8_t idx_back = ENVI_PAGE+1;

void airQuality_disp(){

	init_airQuality_disp();

	while (1){

		speech_airquality_cmd(&speech_command);
		if (THIS_PAGE == ENVI_PAGE)
			airQuality_draw();

		else {
			deinit_airQuality_disp();
			menu_disp();
		}
		vTaskDelay(20);
	}
	deinit_airQuality_disp();
}

static void init_airQuality_disp(){

	button.attachPressed(&btn_obj[3],prev_Cb);

	u8g2_ClearDisplay(&u8g2_obj);
	u8g2_ClearBuffer(&u8g2_obj);

	THIS_PAGE = ENVI_PAGE;
	speech_command = 0;
}

static void deinit_airQuality_disp(){

	//	Melakukan deattach button
	button.dettachPressed(&btn_obj[BUTTON_BACK]);//BACK

	u8g2_ClearDisplay(&u8g2_obj);
	u8g2_ClearBuffer(&u8g2_obj);
}

static void airQuality_getVal(){
#ifdef USE_DPS310
	sprintf(data_buf_pressure  ,"Pres :%0.2f\t hPa",dps_sensor.pressure);
	sprintf(data_buf_temperatur,"Temp :%0.2f\t C\xB0",dps_sensor.temperature);
#endif

#ifdef USE_BMP280
	sprintf(data_buf_pressure  ,"Pres :%0.2f\t hPa",bmp280_sensor.pressure);
	sprintf(data_buf_temperatur,"Temp :%0.2f\t C\xB0",bmp280_sensor.temperature);
#endif

#ifdef USE_BME680
	sprintf(data_buf_gas,"Pres :%0.2f\t hPa",bme680_sensor.gas);
#endif

#ifdef USE_DUMMY_DATA
	sprintf(data_buf_gas,"Pres :%0.2f\t hPa",bme680_sensor.gas);
#endif

}

static void airQuality_draw(){
	airQuality_getVal();
#ifdef USE_DPS310
	u8g2_DrawStr(&u8g2_obj, 0, 10, "Environment field");
	u8g2_DrawStr(&u8g2_obj, 0, 30, data_buf_pressure);
	u8g2_DrawStr(&u8g2_obj, 0, 40, data_buf_temperatur);
#endif

#ifdef USE_BMP280
	u8g2_DrawStr(&u8g2_obj, 0, 10, "Environment field");
	u8g2_DrawStr(&u8g2_obj, 0, 30, data_buf_pressure);
	u8g2_DrawStr(&u8g2_obj, 0, 40, data_buf_temperatur);
#endif

#ifdef USE_BME680
	u8g2_DrawStr(&u8g2_obj, 0, 10, "Gas Data field");
	u8g2_DrawStr(&u8g2_obj, 0, 30, data_buf_gas);
#endif
#ifdef USE_DUMMY_DATA
	u8g2_DrawStr(&u8g2_obj, 0, 10, "Gas Data field");
	u8g2_DrawStr(&u8g2_obj, 0, 30, data_buf_gas);
#endif
	send_buffer_u8g2();
}

static void prev_Cb(){
	THIS_PAGE = idx_back; //index_back
}

static void speech_airquality_cmd(uint8_t *cmd){
	switch(*cmd){

	case BACK_CMD:
		THIS_PAGE = idx_back; //index_back
		break;
	}
}
