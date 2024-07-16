/*
 *
 * main_page.c
 *
 * Created on  : 03 April 2024
 * 	    Author : Ridwan Bahari
 * 	    E-Mail : ridwanbahari236@gmail.com
 *
 */

#include "main_page.h"

#include "rtc_sc.h"
#include "main_ble.h"
#include "task_button.h"
#include "task_sensor.h"
#include "voice_command.h"
#include "task_interface.h"
#include "power_mode.h"

#define MAIN_PAGE_ID 	(1)
#define MENU_PAGE_ID 	(2)
#define SLEEP_PAGE_ID	(3)
#define TIMEOUT			(1ul)

enum mode{
	DEFAULT_MODE,
	BLUETOOTH_MODE
};

struct tm RTC_TIME;

char buf_temp [20];

static uint8_t THIS_PAGE = 0;
static uint8_t MODE_DISPLAY = 0;
//static bool timeout_flag = false;
//static uint32_t minute_timeout = 0;
//static uint32_t second_timeout = 0;

uint8_t koordinatX_oled = 0, koordinatY_oled = 0;

void main_page(){
	init_main_page();

	while (1){
		speech_main_cmd(&speech_command);

		if (THIS_PAGE == MAIN_PAGE_ID)
		{
			if (MODE_DISPLAY == DEFAULT_MODE)
				default_mode_draw();

			else
				bluetooth_mode_draw();
		}
		else if(THIS_PAGE == MENU_PAGE_ID)
		{
			deinit_main_page();
			menu_disp();
			init_main_page();
		}
		else{
			deinit_main_page();
			menu_disp();
			init_main_page();
		}
		vTaskDelay(50);
	}
	deinit_main_page();
}

static void default_mode_draw(){
	disable_ble();

	char buf_time[STRING_BUFFER_SIZE];
	char buf_date[STRING_BUFFER_SIZE];

//	wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL );

	cyhal_rtc_read(&rtc_obj, &RTC_TIME);

	strftime(buf_time, sizeof(buf_time), "%X %p", &RTC_TIME);
	strftime(buf_date, STRING_BUFFER_SIZE, "%b %d, %Y", &RTC_TIME);

#ifdef USE_DUMMY_DATA
	sprintf(buf_temp,"%0.0f C\xB0\r\n", dps_sensor.temperature);
#endif

#ifdef USE_BME680
	sprintf(buf_temp,"%0.0f C\xB0\r\n", bme680_sensor.temperature);
#endif

#ifdef USE_BMP280
	sprintf(buf_temp,"%0.0f C\xB0\r\n", bmp280_sensor.temperature);
#endif

#ifdef USE_DPS310
	sprintf(buf_temp,"%0.0f C\xB0\r\n", dps_sensor.temperature);
#endif

	u8g2_DrawStr(&u8g2_obj, (koordinatX_oled+110), (koordinatY_oled+8), "80%");
	u8g2_DrawStr(&u8g2_obj, (koordinatX_oled+35), (koordinatY_oled+25), buf_time);
	u8g2_DrawStr(&u8g2_obj, (koordinatX_oled+30), (koordinatY_oled+35), buf_date);
	u8g2_DrawStr(&u8g2_obj, (koordinatX_oled+45), (koordinatY_oled+50), buf_temp);

	send_buffer_u8g2();
}

static void bluetooth_mode_draw(){

	wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL );

	u8g2_DrawStr(&u8g2_obj, koordinatX_oled+110, koordinatY_oled+8, "80%");
	u8g2_DrawStr(&u8g2_obj, koordinatX_oled+20, koordinatY_oled+25, "Bluetooth Mode");

	send_buffer_u8g2();
}

static void init_main_page()
{
	// attach button
	button.attachPressed(&btn_obj[BUTTON_ENTER], enter_menu_cb);
	button.attachPressed(&btn_obj[BUTTON_DOWN], main_page_mode);

	THIS_PAGE = MAIN_PAGE_ID;
	MODE_DISPLAY = 0;
	speech_command = 0;
//	timeout_flag = true;
}

static void deinit_main_page(){
	// de attach button
	for (uint8_t i = 0; i < NUM_OF_BTN; i++)
		button.dettachPressed(&btn_obj[i]);

	disable_ble();
}

static void enter_menu_cb(){
//	timeout_flag = true;
	THIS_PAGE = MENU_PAGE_ID;
}

static void main_page_mode(){
//	timeout_flag = true;
	MODE_DISPLAY++;
	if (MODE_DISPLAY > BLUETOOTH_MODE)
		MODE_DISPLAY = DEFAULT_MODE;
}

static void disable_ble(){
	if(wiced_bt_ble_get_current_advert_mode() ==  BTM_BLE_ADVERT_UNDIRECTED_HIGH)
		wiced_bt_start_advertisements( BTM_BLE_ADVERT_OFF, 0, NULL );

	if(connection_id != 0)
		wiced_bt_gatt_disconnect(connection_id);
}

static void speech_main_cmd(uint8_t *cmd){
	switch(*cmd)
	{
	case DEFAULT_MODE_CMD:
//		timeout_flag = true;
		MODE_DISPLAY = DEFAULT_MODE;
		break;

	case BLUETOOTH_MODE_CMD:
//		timeout_flag = true;
		MODE_DISPLAY = BLUETOOTH_MODE;
		break;

	case SHOW_MENU_CMD:
//		timeout_flag = true;
		THIS_PAGE = MENU_PAGE_ID;
		break;
	}
}

//static void time_out(){
//
//	if(timeout_flag)
//	{
//		timeout_flag = false;
//		minute_timeout = (uint32_t)RTC_TIME.tm_min;
//		second_timeout = (uint32_t)RTC_TIME.tm_sec;
//	}
//
//	if((uint32_t)RTC_TIME.tm_min < minute_timeout)
//		minute_timeout = 0;
//
//	if((uint32_t)RTC_TIME.tm_sec < second_timeout)
//		second_timeout = 0;
//
//	if (((uint32_t)RTC_TIME.tm_min - minute_timeout >= (TIMEOUT+1)) && ((uint32_t)RTC_TIME.tm_sec - second_timeout == (TIMEOUT-1)))
//	{
//
//		minute_timeout = (uint32_t)RTC_TIME.tm_min;
//		second_timeout = (uint32_t)RTC_TIME.tm_sec;
//
//		/*Switch to sleep mode/deepsleep mode*/
//        cyhal_syspm_sleep();
//        vTaskDelay(100);
//
//	}
//}
