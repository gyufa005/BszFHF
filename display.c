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

//2 féle irány létezik --> horizontálisnál

SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS][5]; //also szegmensek tombjet reprezentalo tomb
typedef enum {HORIZONTAL,VERTICAL} Alignment;
typedef struct {
  uint8_t x;
  uint8_t y;
}Position;
typedef struct {
  uint8_t segnum; //which segment is on
  Position s_position; //which position in the segment is on
};

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
void MoveHead
