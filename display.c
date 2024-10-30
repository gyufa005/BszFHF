/*
 * display.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Fazekas Gyula
 */
#include "em_device.h"
#include "sl_sleeptimer.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"

sl_sleeptimer_timer_handle_t timer;
static void timeout_callback(sl_sleeptimer_timer_handle_t *handle,void *data);

void display_init(){
  SegmentLCD_Init(false);
  sl_sleeptimer_start_periodic_timer_ms(&timer,500,timeout_callback,NULL,0,SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG);
}
timeout_callback(sl_sleeptimer_timer_handle_t * handle, void * data){
  //Mit csinál mintavételkor
}
//TODO : kirajzolás függvény, és ahhoz a változók struktúrája???
