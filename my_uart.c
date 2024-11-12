#include "em_device.h"
#include "stdbool.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "em_gpio.h"
//getchar uartról kell majd

char lastcharacter = 0;
//Ezt használjuk a csekkoláshoz (J/B-e?)
int _write(int file, char *data, int len) {
int ww;
for (ww=0; ww<len; ww++){
USART_Tx(UART0, data[ww]);
}
return len;
}

//Nem blokkoló UART RX függvény:
//Nem használt
int16_t UART_RXnb(USART_TypeDef *usart)
{
  int16_t retval = -1;
  if(usart->STATUS & USART_STATUS_RXDATAV){
      retval = (int16_t)usart->RXDATA;
  }
  return retval;
}
void UART0_RX_IRQHandler(void){
  lastcharacter = USART_RxDataGet(UART0);
  USART_Tx(UART0,lastcharacter);
  //USART_IntClear(UART0, USART_IF_RXDATAV);//Most kivételesen nem kell
}
//gyak5 copy pastelve
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
