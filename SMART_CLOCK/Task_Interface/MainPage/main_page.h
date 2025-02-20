#ifndef MAIN_PAGE_H_
#define MAIN_PAGE_H_

#include "interface.h"

void main_page();
static void init_main_page();
static void deinit_main_page();
static void default_mode_draw();
static void bluetooth_mode_draw();

static void enter_menu_cb();
static void main_page_mode();
static void enable_ble();
static void disable_ble();
static void speech_main_cmd(uint8_t *cmd);
static void time_out();

extern struct tm RTC_TIME;

#endif
