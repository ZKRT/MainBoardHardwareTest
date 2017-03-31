#include "main.h"
#ifdef __cplusplus
extern "C"
{
#include "sys.h"
#include "adc.h"
#include "usart.h"
#include "flash.h"
#include "led.h"
#include "ostmr.h"
#include "osqtmr.h"	
#include "lwip_comm.h"
#include "lwipapp.h"
#include "usb_usr_process.h"
#include "hwTestHandle.h"
#include "versionzkrt.h"
}
#endif //__cplusplus


#ifdef __cplusplus
extern "C" void ADC_SoftwareStartConv(ADC_TypeDef* ADCx);
#endif //__cplusplus

#ifdef HWTEST_FUN
u8 sys_led_timeout = RUN_LEN_NETINIT_NONE_TIMEOUT;
volatile u8 sys_led_flag = 0;
void app_500ms_task(void)
{
	if(sys_led_flag>1)
	{
		sys_led_flag--;
	}
}
#endif
int main()
{
	BSPinit();
	ZKRT_LOG(LOG_NOTICE, "==================================================\r\n"); 
	printf("PRODUCT_NAME: %s\r\nPRODUCT_ID: %s\r\nPRODUCT_VERSION: %s\r\nPRODUCT_TIME: %s %s\r\n",PRODUCT_NAME,PRODUCT_ID,PRODUCT_VERSION,__DATE__,__TIME__);
	ZKRT_LOG(LOG_NOTICE, "==================================================\r\n"); 
	hwtest_init();
	lwip_prcs_init();
#ifdef HWTEST_FUN	
	t_ostmr_insertTask(app_500ms_task, 500, OSTMR_PERIODIC);
//	ostmr_wait(50);
//	esram_test(); //zkrt_debug
#endif	
	while(1)
	{
		lwip_prcs();
		usb_user_prcs();
	}
}
