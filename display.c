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
#include "my_uart.h"


//2 féle irány létezik --> horizontálisnál

SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];
typedef enum {HORIZONTAL,VERTICAL} Alignment;
typedef enum {NOPE,RIGHT,LEFT} DirectionX;
typedef enum {NOPE,UP,DOWN} DirectionY;

uint16_t*koordinatatomb [5][8] = {
    {lowerCharSegments[0].a,&lowerCharSegments[1].a,&lowerCharSegments[2].a,&lowerCharSegments[3].a,&lowerCharSegments[4].a,&lowerCharSegments[5].a,&lowerCharSegments[6].a,NULL},
    {&lowerCharSegments[0].f,&lowerCharSegments[1].f,&lowerCharSegments[2].f,&lowerCharSegments[3].f,&lowerCharSegments[4].f,&lowerCharSegments[5].f,&lowerCharSegments[6].f,&lowerCharSegments[6].b},
    {&lowerCharSegments[0].g,&lowerCharSegments[1].g,&lowerCharSegments[2].g,&lowerCharSegments[3].g,&lowerCharSegments[4].g,&lowerCharSegments[5].g,&lowerCharSegments[6].g,NULL},
    {&lowerCharSegments[0].e,&lowerCharSegments[1].e,&lowerCharSegments[2].e,&lowerCharSegments[3].e,&lowerCharSegments[4].e,&lowerCharSegments[5].e,&lowerCharSegments[6].e,&lowerCharSegments[6].c},
    {&lowerCharSegments[0].d,&lowerCharSegments[1].d,&lowerCharSegments[2].d,&lowerCharSegments[3].d,&lowerCharSegments[4].d,&lowerCharSegments[5].d,&lowerCharSegments[6].d,NULL}
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
  uint16_t pointer = lowerCharSegments[2].a;
}
timeout_callback(sl_sleeptimer_timer_handle_t * handle, void * data){
  //Mit csinál mintavételkor

  //vmit csinál az aktuális lastcharacter alapján
  //...

  //reseteli az irányt
  lastcharacter = NULL;
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
