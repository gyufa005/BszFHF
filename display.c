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

SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];
typedef enum {HORIZONTAL,VERTICAL} Alignment;
typedef enum {NOP,RIGHT,LEFT} DirectionX;
typedef enum {NOP,UP,DOWN} DirectionY;

uint8_t*koordinatatombuint8_t [8][5] = {
    {&lowerCharSegments[0].a,&lowerCharSegments[1].a,&lowerCharSegments[2].a,&lowerCharSegments[3].a,&lowerCharSegments[4].a,&lowerCharSegments[5].a,&lowerCharSegments[6].a}
};

/*
typedef struct {
  uint8_t x;
  uint8_t y;
}Position;
typedef struct {
  uint8_t segnum; //which segment is on
  Position s_position; //which position in the segment is on
};*/
typedef struct {
  Alignment alignment;
  DirectionX directionx;
  DirectionY directiony;
  SegmentLCD_LowerCharSegments_TypeDef * const segmentpointer; //ez mindig a szegmens tömbre mutat
  uint8_t segnum;
}SnakePart;
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
void MovePart(SnakePart* snakepart){
/*
  //Allapotgep
  if(snakepart->alignment==HORIZONTAL){
  //Ha jobbra megy
  if(snakepart->segmentpointer[snakepart->segnum]->a&snakepart->directionx==RIGHT){
      //set current segment to 0
      snakepart->segmentpointer[snakepart->segnum]->a = 0;
      //change part to new segment
      snakepart->segnum++;
      snakepart->segnum %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;
      //display new segment
      snakepart->segmentpointer[snakepart->segnum]->a = 1;
      return;}
  if(snakepart->segmentpointer[snakepart->segnum]->g&snakepart->directionx==RIGHT){
      snakepart->segmentpointer[snakepart->segnum]->g = 0;
      snakepart->segmentpointer[snakepart->segnum]->m = 0;

      snakepart->segnum++;
      snakepart->segnum %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;

      snakepart->segmentpointer[snakepart->segnum]->g = 1;
      snakepart->segmentpointer[snakepart->segnum]->m = 1;
      return;}
  if(snakepart->segmentpointer[snakepart->segnum]->d&snakepart->directionx==RIGHT){
        snakepart->segmentpointer[snakepart->segnum]->d = 0;
        snakepart->segnum++;
        snakepart->segnum %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;
        snakepart->segmentpointer[snakepart->segnum]->d = 1;
        return;}
  //Ha balra megy
   if(snakepart->segmentpointer[snakepart->segnum]->a&snakepart->directionx==LEFT){
        snakepart->segmentpointer[snakepart->segnum]->a = 0;
        snakepart->segnum--;
        snakepart->segnum %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;
        snakepart->segmentpointer[snakepart->segnum]->a = 1;
        return;}
   if(snakepart->segmentpointer[snakepart->segnum]->g&snakepart->directionx==LEFT){
        snakepart->segmentpointer[snakepart->segnum]->g = 0;
        snakepart->segmentpointer[snakepart->segnum]->m = 0;
        snakepart->segnum--;
        snakepart->segnum %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;
        snakepart->segmentpointer[snakepart->segnum]->g = 1;
        snakepart->segmentpointer[snakepart->segnum]->m = 1;
        return;}
   if(snakepart->segmentpointer[snakepart->segnum]->d&snakepart->directionx==LEFT){
        snakepart->segmentpointer[snakepart->segnum]->d = 0;
        snakepart->segnum--;
        snakepart->segnum %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;
        snakepart->segmentpointer[snakepart->segnum]->d = 1;
        return;}
  //Ha lefele megy
   if(snakepart->segmentpointer[snakepart->segnum]->a&snakepart->directiony==DOWN){
           snakepart->segmentpointer[snakepart->segnum]->a = 0;
           snakepart->segmentpointer;
           return;}
   if(snakepart->segmentpointer[snakepart->segnum]->g&snakepart->directiony==DOWN){
           snakepart->segmentpointer[snakepart->segnum]->g = 0;
           snakepart->segmentpointer[snakepart->segnum]->m = 0;
           snakepart->segmentpointer--;
           return;}
   if(snakepart->segmentpointer->d&snakepart->directiony==DOWN){
           snakepart->segmentpointer[snakepart->segnum]->d = 0;
           snakepart->segmentpointer--;
           return;}

  }
  for(int i=0; i<SEGMENT_LCD_NUM_OF_LOWER_CHARS;i++)

}*/

//Nem tudom a SegmentLCD_Init 0-z-e mindent.
void clearSegments() {
    for (uint8_t i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++) {
        lowerCharSegments[i].raw = 0;  //
    }
}
