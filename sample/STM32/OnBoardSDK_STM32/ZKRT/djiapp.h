

#ifndef _DJIAPP_H
#define _DJIAPP_H



#include "sys.h"											
#include "led.h"											
#include "timer_zkrt.h"										
#include "sbus.h"											
#include "ds18b20.h"									
#include "adc.h"											
#include "iic.h"												
#include "iwatchdog.h"								
#include "usart.h"											
#include "mavlink.h"									
#include "Mavlink_msg_statustext.h" 		
#include "zkrt.h"											
#include "mavlink_msg_smartbat_text.h"	
#include "mavlink_msg_heartbeat.h"		
#include "tempture.h"										
#include "can.h"												
#include "heart.h"											
#include "bat.h"											

extern uint8_t msg_smartbat_dji_buffer[30];

extern uint8_t mavlink_type_flag_dji ;

void main_zkrt_dji_recv(void);
void zkrt_dji_read_heart_tempture(void);
void dji_zkrt_read_heart_vol_check(void);
void dji_bat_value_roll(void);
void dji_zkrt_read_heart_vol(void);
void dji_zkrt_read_heart_tempture_check(void);
void dji_zkrt_read_heart_ack(void);

extern uint16_t ADC1_25_dji;
extern uint16_t ADC1_5_dji;
extern uint16_t ADC1_I_dji;

#endif
