#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h" 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup LED Control Directory
  * @{
  */ 
#define _RUN_LED                                  PEout(4)
#define _ALARM_LED                                PEout(5)
#define _HS_LED                                   PEout(6)
#define _DJI_UART_TX_LED                          PCout(13)  
#define _DJI_UART_RX_LED                          PCout(12)
#define _TEST_UART_TX_LED                         PCout(14)
#define _TEST_UART_RX_LED                         PCout(15)
#define _CAN1_TX_LED                              PAout(5) 
#define _CAN1_RX_LED                              PAout(4)
/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void led_init(void);
void led_test(void);

#endif
