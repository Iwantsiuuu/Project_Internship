#include "write_rtc_task.h"
#include "main_ble.h"
#include "task_sensor.h"

void notify_task(){

	cy_rslt_t rslt;
	char buffer[STRING_BUFFER_SIZE];
	char environtment_buf[STRING_BUFFER_SIZE];
	char air_quality_buf[STRING_BUFFER_SIZE];

	//	char buffer_text[40];
	struct tm date_time;
	//	wiced_result_t wiced_result = WICED_BT_ERROR;

	while(1){

		if(set_rtc){

			rslt = cyhal_rtc_read(&rtc_obj, &date_time);
			if (CY_RSLT_SUCCESS == rslt)
			{
				strftime(buffer, sizeof(buffer), "%c", &date_time);
			}
		}

//#ifdef USE_DUMMY_DATA
//		sprintf(environtment_buf, "P: %0.2f, T: %0.0f, H: %0.0f", bme680_sensor.pressure, bme680_sensor.temperature, bme680_sensor.humidity);
//		sprintf(air_quality_buf, "G: %0.2f",bme680_sensor.gas);
//#endif

		//Send Notify to AIROC
		if(connection_id != 0){
			if(app_psoc_string_client_char_config[0] & GATT_CLIENT_CONFIG_NOTIFICATION){
				//				sprintf(buffer_text,"\r%s\n", buffer);
				wiced_bt_gatt_server_send_notification( connection_id, HDLC_PSOC_STRING_VALUE, strlen(buffer), (uint8_t *)buffer, NULL);
				memset(buffer, '\0', sizeof(buffer));
			}

			if(app_psoc_environment_client_char_config[0] & GATT_CLIENT_CONFIG_NOTIFICATION){

#ifdef USE_DUMMY_DATA
				sprintf(environtment_buf, "P: %0.2f, T: %0.0f, H: %0.0f", bme680_sensor.pressure, bme680_sensor.temperature, bme680_sensor.humidity);
#endif

#ifdef UNUSE_I2S
				printf("%s\r\n",environtment_buf);
#endif
				wiced_bt_gatt_server_send_notification( connection_id, HDLC_PSOC_STRING_VALUE, strlen(environtment_buf), (uint8_t *)environtment_buf, NULL);
				memset(buffer, '\0', sizeof(buffer));
			}

			if(app_psoc_air_quality_client_char_config[0] & GATT_CLIENT_CONFIG_NOTIFICATION){

#ifdef USE_DUMMY_DATA
				sprintf(air_quality_buf, "G: %0.2f",bme680_sensor.gas);
#endif

#ifdef UNUSE_I2S
				printf("%s\r\n",air_quality_buf);
#endif

				wiced_bt_gatt_server_send_notification( connection_id, HDLC_PSOC_STRING_VALUE, strlen(air_quality_buf), (uint8_t *)air_quality_buf, NULL);
				memset(buffer, '\0', sizeof(buffer));
			}
		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
