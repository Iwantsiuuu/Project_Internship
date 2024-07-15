#include "setting_alarm.h"
#include "voice_command.h"

#define ALARM_PAGE 1

#define DEAFULT_VARIABLE_VALUE 		(0)
#define NUM_DAY						(31)
#define NUM_MONTH					(12)
#define NUM_HOUR					(23)
#define NUM_MINUTE					(59)
#define NUM_SECON					(59)

/* Prototype function */
static void init_alarm_disp();
static void deinit_alarm_disp();
static void alarm_draw();

static void increment_var_cb();
static void decrement_var_cb();
static void switch_var_cb();
static void confirm_cb();
static void back_alarm_cb();
static void speech_alarm_cmd(uint8_t *cmd);

/* Global Variable */
static uint8_t THIS_PAGE = 0;
static uint8_t idx_back = ALARM_PAGE+1;
static uint8_t current_var = 0;
static uint8_t num_var = 6;
static bool confirm_flag = false;

enum var_alarm{
	Hour,
	Minute,
	Secon,
	Day,
	Month,
	Year
};

void init_alarm_disp(){

	button.attachPressed(&btn_obj[BUTTON_UP],increment_var_cb);
	button.attachPressed(&btn_obj[BUTTON_DOWN],decrement_var_cb);
	button.attachPressed(&btn_obj[BUTTON_ENTER],switch_var_cb);
	button.attachPressed(&btn_obj[BUTTON_BACK],back_alarm_cb);

	button.attachHeld(&btn_obj[BUTTON_ENTER],confirm_cb);

	u8g2_ClearDisplay(&u8g2_obj);
	u8g2_ClearBuffer(&u8g2_obj);

	THIS_PAGE = ALARM_PAGE;
	speech_command = 0;
}

static void deinit_alarm_disp(){
	//	Melakukan deattach button
	for (uint8_t i = 0; i < NUM_OF_BTN; i++)
		button.dettachPressed(&btn_obj[i]);
}

static void alarm_draw(){
	char buff_HH[10];
	char buff_MM[10];
	char buff_SS[10];
	char buff_dd[10];
	char buff_mm[10];
	char buff_yy[10];

	if(confirm_flag)
		set_alarm();

	sprintf(buff_HH,"HH: %d",RTC_Setup.hour);
	sprintf(buff_MM,"MM: %d", RTC_Setup.min);
	sprintf(buff_SS,"SS: %d", RTC_Setup.sec);
	sprintf(buff_dd,"dd: %d", RTC_Setup.mday);
	sprintf(buff_mm,"mm: %d", RTC_Setup.month);
	sprintf(buff_yy,"yy: %d", RTC_Setup.Year);

	u8g2_DrawStr(&u8g2_obj, 0, 10, "ALARM SETUP");

	u8g2_DrawStr(&u8g2_obj, 2, 30, buff_HH);
	u8g2_DrawStr(&u8g2_obj, 60, 30, buff_dd);

	u8g2_DrawStr(&u8g2_obj, 2, 40, buff_MM);
	u8g2_DrawStr(&u8g2_obj, 60, 40, buff_mm);

	u8g2_DrawStr(&u8g2_obj, 2, 50, buff_SS);
	u8g2_DrawStr(&u8g2_obj, 60, 50, buff_yy);
	send_buffer_u8g2();
}

void alarm_disp(){

	init_alarm_disp();

	while (1){

		speech_alarm_cmd(&speech_command);
		if (THIS_PAGE == ALARM_PAGE)
			alarm_draw();
		else{
			deinit_alarm_disp();
			menu_disp();
		}
		vTaskDelay(20);
	}
	deinit_alarm_disp();
}

static void increment_var_cb(){
	switch (current_var){
	case Hour :
		RTC_Setup.hour++;
		if (RTC_Setup.hour > NUM_HOUR)
			RTC_Setup.hour = DEAFULT_VARIABLE_VALUE;
		break;

	case Minute :
		RTC_Setup.min++;
		if (RTC_Setup.min > NUM_MINUTE)
			RTC_Setup.min = DEAFULT_VARIABLE_VALUE;
		break;

	case Secon :
		RTC_Setup.sec++;
		if (RTC_Setup.sec > NUM_SECON)
			RTC_Setup.sec = DEAFULT_VARIABLE_VALUE;
		break;

	case Day :
		RTC_Setup.mday++;
		if (RTC_Setup.mday > NUM_DAY)
			RTC_Setup.mday = DEAFULT_VARIABLE_VALUE;
		break;

	case Month :
		RTC_Setup.month++;
		if (RTC_Setup.month > NUM_MONTH)
			RTC_Setup.month = DEAFULT_VARIABLE_VALUE;
		break;

	case Year :
		RTC_Setup.Year++;
		break;
	}
}
static void decrement_var_cb(){
	switch (current_var){
	case Hour :
		RTC_Setup.hour--;
		if (RTC_Setup.hour < DEAFULT_VARIABLE_VALUE)
			RTC_Setup.hour = NUM_HOUR;
		break;

	case Minute :
		RTC_Setup.min--;
		if (RTC_Setup.min < DEAFULT_VARIABLE_VALUE)
			RTC_Setup.min = NUM_MINUTE;
		break;

	case Secon :
		RTC_Setup.sec--;
		if (RTC_Setup.sec < DEAFULT_VARIABLE_VALUE)
			RTC_Setup.sec = NUM_SECON;
		break;

	case Day :
		RTC_Setup.mday--;
		if (RTC_Setup.mday < DEAFULT_VARIABLE_VALUE)
			RTC_Setup.mday = NUM_DAY;
		break;

	case Month :
		RTC_Setup.month--;
		if (RTC_Setup.month < DEAFULT_VARIABLE_VALUE)
			RTC_Setup.month = NUM_MONTH;
		break;

	case Year :
		RTC_Setup.Year--;
		break;
	}
}
static void switch_var_cb(){
	current_var++;
	if (current_var > num_var)
		current_var = 0;
}
static void confirm_cb(){
	confirm_flag = true;
}
static void back_alarm_cb(){
	THIS_PAGE = idx_back; //index_back
}

static void speech_alarm_cmd(uint8_t *cmd){

	switch(*cmd){
	case BACK_CMD:
		THIS_PAGE = idx_back; //index_back
	}
}
