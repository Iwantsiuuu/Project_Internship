/*
 * voice_command.h
 *
 *  Created on: 12 June 2024
 *      Author: Ridwan Bahari
 */

#pragma once

#ifndef VOICE_COMMAND_H_
#define VOICE_COMMAND_H_

#include "cyhal.h"
#include "cybsp.h"
#include "stdlib.h"
#include "cy_retarget_io.h"

#include "main.h"
#include "cyberon_asr.h"

#define FRAME_SIZE                  (480u)
#define SAMPLE_RATE_HZ              (16000u)
#define DECIMATION_RATE             (96u)
#define AUDIO_SYS_CLOCK_HZ          (24576000u)
//#define AUDIO_SYS_CLOCK_HZ          (98000000u)
#define PDM_DATA                    (P10_5)
#define PDM_CLK                     (P10_4)

#define	DEFAULT_MODE_CMD 			(21)
#define	BLUETOOTH_MODE_CMD 			(22)
#define	SHOW_MENU_CMD 				(23)
#define	CLOCK_SETTING_CMD 			(24)
#define	ENVIRONMENT_CMD 			(25)
#define	AIR_QUALITY_CMD 			(26)
#define	STOPWATCH_CMD 				(27)
#define	ALARM_SETTING_CMD 			(28)
#define	BACK_CMD 					(29)
#define START_CMD					(30)
#define	STOP_CMD					(31)
#define RESET_CMD					(32)

void init_i2s(void);
void clock_init(void);
void init_pdm_pcm(void);
void voice_command_task(void);

extern uint8_t speech_command;

#ifdef USE_I2S
extern cyhal_i2s_t i2s;
#endif

#endif
