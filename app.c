/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
//LCD-nek
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
//UARTNAK
#include "em_gpio.h"
#include "em_device.h"
#include "stdbool.h"
//timernek
#include "em_cmu.h"
//UART
//#include "display.h"
#include "em_usart.h"
#include <stdlib.h>
#include "sl_sleeptimer.h"
//Random
#include "em_adc.h"


#define GRID_WIDTH 8
#define GRID_HEIGHT 5



/***************************************************************************//**
 * Typedefek, Enumok
 ******************************************************************************/
typedef struct {
  uint8_t x;
  uint8_t y;
}Position_t;


typedef enum {RIGHT,DOWN,LEFT,UP} Direction;
//Snek
typedef struct {
int16_t snakelength;
Position_t  snakeparts[38]; //ennél biztos nem hosszabb
Direction dir;//fejének az iránya
Direction prevdir;//mozgás logikához
}Snake;

/***************************************************************************//**
 * Globals
 ******************************************************************************/
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];
SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS];
volatile char lastcharacter;
Position_t food;
Snake snake;
uint8_t score = 0;
volatile int isDotOn=0;
int geckoOn = 0;


/***************************************************************************//**
 * Initialization
 ******************************************************************************/

void initsnek(Snake* snake){
  snake->snakelength = 1;
  snake->snakeparts[0].x = 0;
  snake->snakeparts[0].y = 2;
  snake->snakeparts[1].x = 6;//Láthatatlan farok, hogy lehessen erre állitani az új részeket.
  snake->snakeparts[1].y = 2;
  snake->dir = RIGHT;
  snake->prevdir = RIGHT;
}

/***************************************************************************//**
 * UART
 ******************************************************************************/
void my_uart_init(void)
{
// Enable clock for GPIO
CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
// Set PF7 to high
GPIO_PinModeSet(gpioPortF, 7, gpioModePushPull, 1);
// Configure UART0
// (Now use the "emlib" functions whenever possible.)
// Enable clock for UART0
CMU_ClockEnable(cmuClock_UART0, true);
// Initialize UART0 (115200 Baud, 8N1 frame format)
// To initialize the UART0, we need a structure to hold
// configuration data. It is a good practice to initialize it with
// default values, then set individual parameters only where needed.
USART_InitAsync_TypeDef UART0_init = USART_INITASYNC_DEFAULT;
USART_InitAsync(UART0, &UART0_init);
// USART0: see in efm32ggf1024.h
// Set TX (PE0) and RX (PE1) pins as push-pull output and input resp.
// DOUT for TX is 1, as it is the idle state for UART communication
GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1);
// DOUT for RX is 0, as DOUT can enable a glitch filter for inputs,
// and we are fine without such a filter
GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);
// Use PE0 as TX and PE1 as RX (Location 1, see datasheet (not refman))
// Enable both RX and TX for routing
UART0->ROUTE |= UART_ROUTE_LOCATION_LOC1;
// Select "Location 1" as the routing configuration
UART0->ROUTE |= UART_ROUTE_TXPEN | UART_ROUTE_RXPEN;

//Interrupt enable
//UART0 vételi kérés megszakítás kérés felkonfigurálása
  USART_IntEnable(UART0,USART_IF_RXDATAV);
  NVIC_EnableIRQ(UART0_RX_IRQn);
}
/***************************************************************************//**
 * Timer
 ******************************************************************************/
volatile char lastcharacter = '0';
sl_sleeptimer_timer_handle_t timer;
static void app_timeout_callback(sl_sleeptimer_timer_handle_t* timer,void* data);
/***************************************************************************//**
 * Random Number Generator
 ******************************************************************************/
void initADC ()
{

  //INIT
    // Enable ADC0 clock
    CMU_ClockEnable(cmuClock_ADC0, true);

    // Declare init structures
    ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
    ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

    // Modify init structures and initialize
    init.prescale = ADC_PrescaleCalc(7000000, 0);

    initSingle.diff       = false;        // single ended
    initSingle.reference  = adcRef2V5;    // internal 2.5V reference
    initSingle.resolution = adcRes12Bit;  // 12-bit resolution

    // Select ADC input
    initSingle.input = adcSingleInputTemp;
    init.timebase = ADC_TimebaseCalc(0);

    ADC_Init(ADC0, &init);
    ADC_InitSingle(ADC0, &initSingle);
}
int startADC (void)
{
    /*Start ADC conversion*/
    ADC_Start(ADC0, adcStartSingle);
    /*Wait for conversion to be complete*/
    while(ADC0->STATUS & ADC_STATUS_SINGLEACT);
    /*Get ADC result*/
    int x = ADC_DataSingleGet(ADC0);

    return x;
}


/***************************************************************************//**
 * Functions
 ******************************************************************************/
void initSnekMove()
{
  Position_t tailtarget = snake.snakeparts[snake.snakelength-1];
    snake.snakeparts[snake.snakelength]=tailtarget;
              snake.snakelength++;
             for(int i = snake.snakelength-2;i>0;i--)
                 {
                                snake.snakeparts[i]=snake.snakeparts[i-1];
                 }
 }

void snakedirection(char newdir){
  snake.prevdir=snake.dir;
  //head direction
  if (newdir == 'j')
    {
     snake.dir++;
     snake.dir%=4;
    }
  if(newdir == 'b')
    {
      if(snake.dir==RIGHT){
          snake.dir=UP;
      }
      else{
          snake.dir--;
      }
    }
  return;
}

void placeFood() {
  int temp = abs(startADC());
  temp%=64;
    food.y = temp % GRID_HEIGHT;
      if(food.y==1||food.y==3)    //ha fuggoleges
        food.x = temp % GRID_WIDTH;
      else
        food.x = temp % GRID_WIDTH-1;

}
bool isFoodEaten() {
  bool retval;
  retval =(snake.snakeparts[0].x == food.x) && (snake.snakeparts[0].y == food.y);
  return retval;
}
bool isBitingItself(Snake* snake){
  for(int i=2;i<snake->snakelength;i++){
      if(snake->snakeparts[0].y==snake->snakeparts[i]->y&&snake->snakeparts[0].x==snake->snakeparts[i].x)
        return true;
  }
  return false;
}
//Keresztbeharapja-e magát a kigyó
bool isCrossBiting(){
  Snake simulatedSnake = snake;

      // Jobbra kanyarodna
      simulatedSnake.prevdir = simulatedSnake.dir;
      simulatedSnake.dir = (simulatedSnake.dir + 1) % 4;  // kanyar jobbra
      SnakeMove(&simulatedSnake);
      if (isBitingItself(&simulatedSnake)) {
          return true;
      }

      // Balra kanyarodna
      simulatedSnake = snake;
      simulatedSnake.prevdir = simulatedSnake.dir;
      simulatedSnake.dir = (simulatedSnake.dir + 3) % 4;  // balra
      SnakeMove(&simulatedSnake);
      if (isBitingItself(&simulatedSnake)) {
          return true;
      }

      return false;  // No cross-biting detected

}


void SnakeMove(Snake * snake)
{
  //Másolat ha kéne felfűzni új elemet
    Position_t tailtarget = snake->snakeparts[snake->snakelength-1];
  //Azonos irany
    if(snake->dir==snake->prevdir)
      {
      switch (snake->dir){
        case RIGHT:
          {
            snake->snakeparts[0].x++;break;
          }
        case DOWN:
          {
            snake->snakeparts[0].y+=2;break;
          }
        case LEFT:
          {
            snake->snakeparts[0].x--;break;
          }
        case UP:
          {
            snake->snakeparts[0].y-=2;break;
          }
      }
    }
    if(snake->prevdir==RIGHT)
      {//
      if(snake->dir==UP){//->^
              snake->snakeparts[0].x++;
              snake->snakeparts[0].y--;
      }
      if(snake->dir==DOWN){//->ˇ
              snake->snakeparts[0].x++;
              snake->snakeparts[0].y++;
    }
    }
    if(snake->prevdir==DOWN){
        if(snake->dir==LEFT){//ˇ<-
            snake->snakeparts[0].x--;
            snake->snakeparts[0].y++;
        }
        if(snake->dir==RIGHT){//ˇ->
            snake->snakeparts[0].y++;
        }
    }
    if(snake->prevdir==LEFT){
        if(snake->dir==DOWN){//<-ˇ
            snake->snakeparts[0].y++;
        }
        if(snake->dir==UP){//<-^
            snake->snakeparts[0].y--;
        }
    }
    if(snake->prevdir==UP){
        if(snake->dir==LEFT){//^<-
            snake->snakeparts[0].x--;
            snake->snakeparts[0].y--;
        }
        if(snake->dir==RIGHT){//^->
            snake->snakeparts[0].y--;
        }
    }

      //Meg kell nézni, hogy ha a fej eléri a szélét a pályának, akkor
      if((snake->snakeparts[0].y==1)||(snake->snakeparts[0].y==3)||(snake->snakeparts[0].y==5)||(snake->snakeparts[0].y==0xFF))//Ha vizszintesen van
        {
          if(snake->snakeparts[0].y==5)
                  {
                    snake->snakeparts[0].y=1;
                  }
                if(snake->snakeparts[0].y==0xFF)
                  {
                    snake->snakeparts[0].y=3;
                  }

        }
      else{
          if(snake->snakeparts[0].x==0xFF)//alulcsordul mert unsigned
                            {
                              snake->snakeparts[0].x=6;
                            }
          if(snake->snakeparts[0].x>=7)
                  {
                    snake->snakeparts[0].x=0;
                  }

      }
//Fej lépett


      //Ha hosszabbodott máshogy kell léptetni a többi részét

      if(isFoodEaten()){
          snake->snakeparts[snake->snakelength]=tailtarget;
          score++;
          placeFood();
          geckoOn = 1;
          SegmentLCD_Symbol(LCD_SYMBOL_GECKO, geckoOn);
          geckoOn = 0;
          snake->snakelength++;
         for(int i = snake->snakelength-2;i>0;i--)
             {
                            snake->snakeparts[i]=snake->snakeparts[i-1];
             }
      }
      else
      {
                  for(int i = snake->snakelength;i>0;i--)
                                {
                                    snake->snakeparts[i]=snake->snakeparts[i-1];
                                }
      }





}
/***************************************************************************//**
 * Display functions.
 ******************************************************************************/
void ClearDisplay(){
  //Lower Display clear
      for (uint8_t i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++) {
          lowerCharSegments[i].raw = 0;  //
      }
  //Upper Display clear
      for (uint8_t i = 0; i < SEGMENT_LCD_NUM_OF_UPPER_CHARS; i++) {
                upperCharSegments[i].raw = 0;  //
}
}


void EndOfGame(){
  if(isDotOn){
          isDotOn=0;
      }
      else {
        isDotOn = 1;
      }
  //Tizedes pontok villognak. (minden órajelnél negáljuk)
    SegmentLCD_Symbol(LCD_SYMBOL_DP2, isDotOn);
  SegmentLCD_Symbol(LCD_SYMBOL_DP3, isDotOn);
  SegmentLCD_Symbol(LCD_SYMBOL_DP4, isDotOn);
  SegmentLCD_Symbol(LCD_SYMBOL_DP5, isDotOn);
  SegmentLCD_Symbol(LCD_SYMBOL_DP6, isDotOn);

}
void DisplayScore(int score){
  ClearDisplay();
  SegmentLCD_Number(score);
}
void DisplayPos(Position_t pos){
  if(pos.x == 7){
      switch(pos.y){
        case 1:lowerCharSegments[6].c = true;break;
        case 3:lowerCharSegments[6].b = true;break;
      }
  }
  else {
      switch(pos.y){
        case 0:
                lowerCharSegments[pos.x].a = true; break;
              case 1:
                lowerCharSegments[pos.x].f = true; break;
              case 2:
                lowerCharSegments[pos.x].g = true;
                lowerCharSegments[pos.x].m = true; break;
              case 3:
                lowerCharSegments[pos.x].e = true; break;
              case 4:
                lowerCharSegments[pos.x].d = true; break;
              default: break;
      }
  }
}
void Display(){
  DisplayScore(score);
  DisplayPos(food);
  for(int i=0;i<snake.snakelength;i++){
      DisplayPos(snake.snakeparts[i]);
  }
  SegmentLCD_LowerSegments(lowerCharSegments);
}
/***************************************************************************//**
 * Snake Automation
 ******************************************************************************/
//BFS

typedef struct {
  Position_t pos;
  int stepsneeded;
}BFSNode;


/***************************************************************************//**
 * App Initialization
 ******************************************************************************/

void app_init(void)
{
  initADC();
  my_uart_init();
  SegmentLCD_Init(false);
  initsnek(&snake);
  sl_sleeptimer_start_periodic_timer_ms(&timer, 500, app_timeout_callback, NULL, 0, SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG);
  initSnekMove();
  placeFood();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}
/***************************************************************************//**
 * Interrupt functions.
 ******************************************************************************/
//ezt csinálja minden egyes ticken
void app_timeout_callback(sl_sleeptimer_timer_handle_t* timer,void* data){

  if(isBitingItself(&snake)){
      EndOfGame();
  }
  else{
      snakedirection(lastcharacter);
      lastcharacter = '0';
      Display();
      SegmentLCD_Symbol(LCD_SYMBOL_GECKO, geckoOn);
      SnakeMove(&snake);

  }
}
void UART0_RX_IRQHandler(void){

      lastcharacter = USART_RxDataGet(UART0);
      //USART_Tx(UART0,lastcharacter);

}
