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
#include "my_uart.h"
#include "sl_sleeptimer.h"
timeout_callback(sl_sleeptimer_timer_handle_t * handle, void * data){
  //Mit csinál mintavételkor

  //vmit csinál az aktuális lastcharacter alapján
  //...

  //reseteli az irányt
  lastcharacter = NULL;
}
void app_init(void)
{
  my_uart_init();
  display_init();
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
