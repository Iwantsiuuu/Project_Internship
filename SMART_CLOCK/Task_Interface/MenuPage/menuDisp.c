#include "menuDisp.h"

#include "menu.h"
#include "rtc_sc.h"
#include "main_page.h"
#include "timeDatePage.h"
#include "setting_alarm.h"
#include "voice_command.h"
#include "airQualityPage.h"
#include "EnvironmentPage.h"

#define tot_menu (5)
#define TIMEOUT  (1)

static void up_Cb();
static void down_Cb();
static void ok_Cb();
static void back_Cb();
static void speech_menu_cmd(uint8_t *cmd);
static void time_out();

menu_t menuObj;
u8g2_t u8g2;

static uint32_t minute_timeout = 0;
static uint32_t second_timeout = 0;
static bool timeout_flag = false;
static uint8_t menu_cursor;
static uint8_t index_back = tot_menu;

typedef enum {
	TIME_DATE,
	ENVIRONMENT,
	AIRQUALITY,
	STOPWATCH,
	ALARM
}menu_var;

const char *menTest[]={
		"TIME AND DATE",
		"ENVIRONMENT",
		"AIR QUALITY",
		"STOPWATCH",
		"ALARM"
};

const char title_menu[] = {
		"MENU PAGE"
};

void init_menu_disp(){
	button.attachPressed(&btn_obj[BUTTON_UP],up_Cb);
	button.attachPressed(&btn_obj[BUTTON_DOWN],down_Cb);
	button.attachPressed(&btn_obj[BUTTON_ENTER],ok_Cb);
	button.attachPressed(&btn_obj[BUTTON_BACK],back_Cb);

	interface_construct(&menuObj, &u8g2);
	interface_begin(&menuObj);
	interface_setTitle(&menuObj, title_menu);
	interface_set_menu(&menuObj, menTest, tot_menu);
	interface_clearAll(&menuObj);
	menu_cursor=255;
	speech_command = 0;
	timeout_flag = true;
}

void deinit_menu_disp(){
	//	Melakukan deattach button
	for (uint8_t i = 0; i < NUM_OF_BTN; i++)
		button.dettachPressed(&btn_obj[i]);
	interface_clearAll(&menuObj);
}

void menu_enter(uint8_t *disp){
	switch(*disp){
	case TIME_DATE :
		//		Function setting time and date
		rtc_disp();
		break;
	case ENVIRONMENT :
		//		Function BMP Value
		environment_disp();
		break;
	case AIRQUALITY :
		//		Function PASCO2
		airQuality_disp();
		break;
	case STOPWATCH :
		//		Function StopWatch
		stopWatch_disp();
		break;

	case ALARM:
		alarm_disp();
		break;
	}
}

void draw_menu(){
	interface_draw_menu(&menuObj);
	send_buffer_u8g2();
}

void menu_disp(){

	init_menu_disp();

	while (1){
		speech_menu_cmd(&speech_command);
		if (menu_cursor != 255){
			if (menu_cursor == index_back){
				u8g2_ClearBuffer(&u8g2);
				main_page();
				break;
			}
			else{
				deinit_menu_disp();
				menu_enter(&menu_cursor);
				init_menu_disp();
			}
		}
		else
			draw_menu();
		vTaskDelay(20);
	}
	deinit_menu_disp();
}

static void up_Cb(){
	interface_previous(&menuObj);
	timeout_flag = true;

}

static void down_Cb(){
	interface_next(&menuObj);
	timeout_flag = true;

}

static void ok_Cb(){
	menu_cursor = interface_getPosition(&menuObj);
	timeout_flag = true;

}

static void back_Cb(){

	//	Jika page sedang di dalam menu setting jam/tanggal maka akan kembali ke menu page
	menu_cursor = index_back; //index_back
}

static void speech_menu_cmd(uint8_t *cmd){
	switch(*cmd)
	{
	case CLOCK_SETTING_CMD:
		menu_cursor = TIME_DATE;
		break;

	case ENVIRONMENT_CMD:
		menu_cursor = ENVIRONMENT;
		break;

	case AIR_QUALITY_CMD:
		menu_cursor = AIRQUALITY;
		break;

	case STOPWATCH_CMD:
		menu_cursor = STOPWATCH;
		break;

	case ALARM_SETTING_CMD:
		menu_cursor = ALARM;
		break;

	case BACK_CMD:
		menu_cursor = index_back;
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
//	if (((uint32_t)RTC_TIME.tm_min - minute_timeout >= (TIMEOUT+1)) && ((uint32_t)RTC_TIME.tm_sec - second_timeout == (TIMEOUT-1))){
//		minute_timeout = (uint32_t)RTC_TIME.tm_min;
//		second_timeout = (uint32_t)RTC_TIME.tm_sec;
//		menu_cursor = index_back;
//#ifdef UNUSE_I2S
//		printf("Switch to sleep mode/deepsleep mode\r\n");
//#endif
//	}
//}
