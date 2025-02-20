#ifndef MAIN_BLE_H_
#define MAIN_BLE_H_

#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"

/* Library for malloc and free */
#include "stdlib.h"

/* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

/* btstack */
#include "wiced_bt_stack.h"

/* App utilities */
#include "app_bt_utils.h"

/* Include header files from BT configurator */
#include "cycfg_bt_settings.h"
#include "cycfg_gap.h"
#include "cycfg_gatt_db.h"
#include "rtc_sc.h"
#include <string.h>

#define APP_TIMEOUT_LED_BLINK          (200)

/* Typdef for function used to free allocated buffer to stack */
typedef void (*pfn_free_buffer_t)(uint8_t *);

/*******************************************************************
 * Function Prototypes
 ******************************************************************/
/* Callback function for Bluetooth stack management type events */
void app_bt_timeout_led_blink(TimerHandle_t timer_handle);
void app_bt_led_blink(uint16_t num_of_blinks);
wiced_bt_dev_status_t app_bt_management_callback             (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);

extern cyhal_pwm_t PWM_obj;
extern TimerHandle_t timer_led_blink;

extern uint8_t event_data_ble;

extern uint16_t connection_id;
extern uint16_t reason_dic;

extern bool set_rtc;
extern bool flag_bt_actived;

#endif
