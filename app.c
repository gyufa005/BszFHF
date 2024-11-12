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
//UARTNAK
#include "em_gpio.h"
#include "em_device.h"
#include "stdbool.h"
//timernek
#include "em_cmu.h"
//UART
//#include "my_uart.h"
#include "sl_sleeptimer.h"
#include "display.h"

#define GRID_WIDTH 8
#define GRID_HEIGHT 5
//UART
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




//Timer
sl_sleeptimer_timer_handle_t timer;

 void timeout_callback(sl_sleeptimer_timer_handle_t * handle, void * data){
  //Mit csinál mintavételkor

  //vmit csinál az aktuális lastcharacter alapján
  //...

  //reseteli az irányt
  lastcharacter = 0;
}
typedef struct {
  uint8_t x;
  uint8_t y;
}Position_t;
typedef struct {
  Position_t pos;
  uint16_t segments;
}Entity_t;



typedef enum {RIGHT,DOWN,LEFT,UP} Snek_Direction;
//Snek
Entity_t snake[16]; //mondj legyen 16 a max hossz
uint8_t snakelength;
Snek_Direction dir;
Position_t food;
void initsnek(void){
  snake[0].pos.x = 0;
  snake[0].pos.y = 3;
  snakelength = 1;
  Snek_Direction dir = RIGHT;
}
void placeFood() {
    food.y = rand() % GRID_HEIGHT;
    if(food.y==1||food.y==3)    //ha fuggoleges
      food.x = rand() % GRID_WIDTH;
    else
      food.x = rand() % GRID_WIDTH-1;

}
bool isFoodEaten() {
    return snake[0].pos.x == food.x && snake[0].pos.y == food.y;
}

void app_init(void)
{
  my_uart_init();
  display_init();
  initsnek();
  sl_sleeptimer_start_periodic_timer_ms(&timer,500,timeout_callback,NULL,0,SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG);

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  /*while(1){
      int16_t ch = UART_RXnb(UART0);
      if(ch!=-1)
        USART_Tx(UART0);
  }*/
}
