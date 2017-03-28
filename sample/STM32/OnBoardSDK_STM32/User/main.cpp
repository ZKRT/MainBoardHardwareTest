#include "main.h"
#include "stm32f4xx_adc.h"
#include "DJI_API.h"
#include "DJI_Camera.h"
#include "DJI_Type.h"
#include "DJI_VirtualRC.h"
#ifdef __cplusplus
extern "C"
{
#include "sys.h"
#include "adc.h"
#include "usart.h"
#include "zkrt.h"
#include "djiapp.h"
#include "flash.h"
#include "bat.h"
#include "mobileDataHandle.h"
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

#undef USE_ENCRYPT
/*-----------------------DJI_LIB VARIABLE-----------------------------*/
using namespace DJI::onboardSDK;

HardDriver* driver = new STM32F4;
CoreAPI defaultAPI = CoreAPI(driver);
CoreAPI *coreApi = &defaultAPI;

Flight flight = Flight(coreApi);

FlightData flightData;
Camera camera=Camera(coreApi);
GimbalSpeedData gimbalSpeedData;
VirtualRC virtualrc = VirtualRC(coreApi);

VirtualRCData myVRCdata =
{
  1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
  1024, 1024
};

uint32_t runOnce = 1;
uint32_t next500MilTick;

extern TerminalCommand myTerminal;
extern LocalNavigationStatus droneState;
extern uint8_t myFreq[16];
//zkrt_packet_t main_dji_rev;
//uint8_t j=0;
//uint8_t djidataformmobile[50]= {0};
uint8_t data[50]= {0};
uint8_t tempture_invalid=0; //两个温度值是否有效，无效为1，有效为0

uint8_t msgbuffer[25] = {0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77};

#ifdef __cplusplus
extern "C" void ADC_SoftwareStartConv(ADC_TypeDef* ADCx);
#endif //__cplusplus

/////////////////////////////////////////////////////// static function define
void dji_init(void);
void dji_process(void);

/***********************************************************************
		          中科瑞泰消防无人机函数定义

		函数 名称： 数据透传
	       函数 功能：
		函数 入参：
		函数返回值：
		说     明：

************************************************************************/
extern "C" void sendToMobile(uint8_t *data, uint8_t len)
{
  coreApi->sendToMobile(data,  len);//数据透传到地面站软件
  GPIO_ResetBits(GPIOD, GPIO_Pin_0);
  usart1_tx_flag = TimingDelay;
}
extern "C" void sendpoll()
{
  coreApi->sendPoll();//
}
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

//int main()
//{
//  BSPinit();
//  delay_nms(20000);	
//	dji_init();
//  while (1)
//  {
//		dji_process();
//#if 1
//		mobile_data_process();
//    main_zkrt_dji_recv();	/*从子模块通过CAN接收数据，将毒气、抛投数据填充到心跳包，并且相关位置一*/
//#endif

//    if (_160_read_flag - TimingDelay >= 160)
//    {
//      _160_read_flag = TimingDelay;
//      ADC_SoftwareStartConv(ADC1); /*启动ADC*/
//      printf("ADC_SoftwareStartConv!\r\n");
//      if ((_read_count%2) == 0)
//      {
//				
//#ifndef _TEMPTURE_NONE				
////////温度过高控制飞机上升逻辑处理	[START] //modify by yanly
//        zkrt_dji_read_heart_tempture();  /*获取温度传感器数据*/
//        if(((tempture0>0)||(tempture1>0))&&(s==0))
//        {
//          //				printf("####################!\r\n");
//          //			coreApi->getObtainControlMobileCMD();

//          //					printf("111111111111111111111111111!\r\n");
//          //			coreApi->getTakeOffMobileCMD();
//          //                                printf("2222222222222222222222222!\r\n");
//          //                          printf("mobileCommandHandler!\r\n");
//          //                  mobileCommandHandler(coreApi, &flight);
//          //					   printf("333333333333333333333!\r\n");
//          //				     delay_nms(1000);
//          //	coreApi->getObtainControlMobileCMD_dji();
//          //	 printf("44444444444444444444444!\r\n");
//          //	coreApi->getTakeOffMobileCMD_dji();
//          //			 printf("55555555555555555555!\r\n");
//          //          delay_nms(5000);
//          virtualrc.setControl(1,virtualrc.CutOff_ToRealRC);
//          myVRCdata=virtualrc.getVRCData();

//          //		printf("myVRCdata. roll= %d\r\n",myVRCdata.roll);
//          //		printf("myVRCdata.pitch= %d\r\n",myVRCdata.pitch);
//          //		printf("myVRCdata.throttle= %d \r\n",myVRCdata.throttle);
//          //		printf("myVRCdata.yaw= %d\r\n",myVRCdata.yaw);
//          //		printf("myVRCdata.gear= %d\r\n",myVRCdata.gear);
//          //		printf("myVRCdata.reserved= %d\r\n",myVRCdata.reserved);
//          //		printf("myVRCdata.mode= %d\r\n",myVRCdata.mode);
//          //		printf("myVRCdata.Channel_07= %d\r\n",myVRCdata.Channel_07);
//          //		printf("myVRCdata.Channel_08= %d\r\n",myVRCdata.Channel_08);
//          //		printf("myVRCdata.Channel_09= %d\r\n",myVRCdata.Channel_09);
//          //		printf("myVRCdata.Channel_10=%d\r\n",myVRCdata.Channel_10);
//          //		printf("myVRCdata.Channel_11=%d\r\n",myVRCdata.Channel_11);
//          //		printf("myVRCdata.Channel_12= %d\r\n",myVRCdata.Channel_12);
//          //		printf("myVRCdata.Channel_13= %d\r\n",myVRCdata.Channel_13);
//          //		printf("myVRCdata.Channel_14=  %d\r\n",myVRCdata.Channel_14);
//          //		printf("myVRCdata.Channel_15=  %d\r\n",myVRCdata.Channel_15);
//          myVRCdata.throttle=1500; //why? //yanly
//          //			printf("myVRCdata.throttle= %d \r\n",myVRCdata.throttle);
//          virtualrc.sendData(myVRCdata); //为什么要发送vrc命令？//by yanly
//          //			 printf("777777777777777777777777!\r\n");
//          //			coreApi->getReleaseControlMobileCMD();
//          //                mobileCommandHandler(coreApi, &flight);
//          //			coreApi->getReleaseControlMobileCMD_dji();
//          //		s=1;
//        }
//        if((tempture0<0)&&(tempture1<0))
//          s=1;
////////温度过高控制飞机上升逻辑处理	[END]		
//#endif
//				
////        if (MAVLINK_TX_INIT_VAL - TimingDelay >= 4000) //modify by yanly
////        {
////          dji_zkrt_read_heart_tempture_check();	/*将温度传感器数据填充到心跳包里面，并判断数据是否正常*/
////        }
//      }
//      if ((_read_count%10) == 0)
//      {
////        dji_bat_value_roll(); /*获取智能电池数据，并填充到智能电池数据数组中*/  //modify by yanly
//        dji_zkrt_read_heart_vol(); /*获取板载电压电流值*/

//        if (MAVLINK_TX_INIT_VAL - TimingDelay >= 4000)
//        {
//          dji_zkrt_read_heart_vol_check();	/*对当前板载电压电流值进行判断，并作出相应操作，将结果填充到心跳包*/
//        }
//      }
//      _read_count++;
//    }
//    if (MAVLINK_TX_INIT_VAL - TimingDelay >= 5000)  //5s延时到之后一直处理以下逻辑
//    {
//			////////// RUN LED灯管理
//      if ((mavlink_send_flag-TimingDelay) < 50)
//      {
//        _RUN_LED = LED_LIGHT;
//      }
//      else if ((mavlink_send_flag-TimingDelay) < 800)
//      {
//        _RUN_LED = LED_DARK;
//      }
//			//////////
//			
//			/////////与mobile的心跳包处理
//      else if ((mavlink_send_flag-TimingDelay) >= 800)
//      {
//        mavlink_send_flag = TimingDelay;
//#if 1
//        switch (mavlink_type_flag_dji)
//        {
//        case 0:
//          mavlink_type_flag_dji = 1;
//          dji_zkrt_read_heart_ack(); /*完成最终的数据包，并且发送出处*/
//          coreApi->sendPoll();
//          break;
//        case 1:
//          mavlink_type_flag_dji = 0;
////          dji_bat_value_send();  /*发送智能电池数据，总共32个字节*/    //????为什么没有封装zkrt packet格式 //modify by yanly
//          coreApi->sendPoll();
//          break;
//        }
//#endif
//        printf("T1 = %d T2 = %d  ADC1_25_dji=%d  ADC1_5_dji=%d  ADC1_I_dji=%d   LOW_VALUE = %d HIGH_VALUE = %d STA1 = %x STA2 = %x\r\n",
//               tempture0, tempture1, ADC1_25_dji,ADC1_5_dji,ADC1_I_dji,glo_tempture_low, glo_tempture_high, msg_smartbat_dji_buffer[0],
//               msg_smartbat_dji_buffer[3]);
//      }
//    }

////////串口1 LED灯管理
//    if ((usart1_tx_flag-TimingDelay) >= 50)
//    {
//      _FLIGHT_UART_TX_LED = LED_DARK;
//    }

//    if ((usart1_rx_flag-TimingDelay) >= 50)
//    {
//      _FLIGHT_UART_RX_LED = LED_DARK;
//    }
////////
//    IWDG_Feed();
//  }
//}

/*
* description: Old logic for fire protection. //modify by yanly
*/
//int main()
//{
//  BSPinit();
//  delay_nms(20000);
//  printf("This is the example App to test DJI onboard SDK on STM32F4Discovery Board! \r\n");
//  printf("Refer to \r\n");
//  printf("https://developer.dji.com/onboard-sdk/documentation/github-platform-docs/STM32/README.html \r\n");
//  printf("for supported commands!\r\n");
//  printf("Board Initialization Done!\r\n");
//	
//  //! Change the version string to your platform/version as defined in DJI_Version.h
//  coreApi->setVersion(versionA3_31);
//  delay_nms(200);
//  printf("API Version Set Done!\r\n");

//  uint32_t runOnce = 1;
//  uint32_t next500MilTick;
////  uint16_t cm;
//  while (1)
//  {
//    // One time automatic activation
//    if (runOnce)
//    {
//      runOnce = 0;
//      coreApi->setBroadcastFreq(myFreq);
//      delay_nms(50);

//      // The Local navigation example will run in broadcast call back function,
//      // immediate after GPS position is updated
//      coreApi->setBroadcastCallback(myRecvCallback,(DJI::UserData)(&droneState));

//      User_Activate();
//      delay_nms(50);

//      next500MilTick = driver->getTimeStamp() + 500;
//    }

//    //   virtualrc.sendData(myVRCdata);
//    // virtualrc.setControl(0,virtualrc.CutOff_ToLogic);

//    if (driver->getTimeStamp() >= next500MilTick)
//    {
//      next500MilTick = driver->getTimeStamp() + 500;

//      // Handle user commands from mobile device
//      mobileCommandHandler(coreApi, &flight);

//      // Handle user commands from serial (USART2)
//      //   myTerminal.terminalCommandHandler(coreApi, &flight);
//    }
//    coreApi->sendPoll();
//#if 1
//    //获取从地面站软件的数据
//    if(djidataformmobile[1]!=0)
//    {
//      copydataformmobile();
//      main_dji_recv();
//      djidataformmobile[1]	  =0;
//    }
//    main_zkrt_dji_recv();	/*从子模块通过CAN接收数据，将毒气、抛投数据填充到心跳包，并且相关位置一*/

//#endif

//    if (_160_read_flag - TimingDelay >= 160)
//    {
//      _160_read_flag = TimingDelay;
//      ADC_SoftwareStartConv(ADC1); /*启动ADC*/
//      printf("ADC_SoftwareStartConv!\r\n");
//      if ((_read_count%2) == 0)
//      {

//        zkrt_dji_read_heart_tempture();  /*获取温度传感器数据*/
//        if(((tempture0>0)||(tempture1>0))&&(s==0))
//        {
//          //				printf("####################!\r\n");
//          //			coreApi->getObtainControlMobileCMD();

//          //					printf("111111111111111111111111111!\r\n");
//          //			coreApi->getTakeOffMobileCMD();
//          //                                printf("2222222222222222222222222!\r\n");
//          //                          printf("mobileCommandHandler!\r\n");
//          //                  mobileCommandHandler(coreApi, &flight);
//          //					   printf("333333333333333333333!\r\n");
//          //				     delay_nms(1000);
//          //	coreApi->getObtainControlMobileCMD_dji();
//          //	 printf("44444444444444444444444!\r\n");
//          //	coreApi->getTakeOffMobileCMD_dji();
//          //			 printf("55555555555555555555!\r\n");
//          //          delay_nms(5000);
// //         virtualrc.setControl(1,virtualrc.CutOff_ToRealRC);
//          myVRCdata=virtualrc.getVRCData();

//          //		printf("myVRCdata. roll= %d\r\n",myVRCdata.roll);
//          //		printf("myVRCdata.pitch= %d\r\n",myVRCdata.pitch);
//          //		printf("myVRCdata.throttle= %d \r\n",myVRCdata.throttle);
//          //		printf("myVRCdata.yaw= %d\r\n",myVRCdata.yaw);
//          //		printf("myVRCdata.gear= %d\r\n",myVRCdata.gear);
//          //		printf("myVRCdata.reserved= %d\r\n",myVRCdata.reserved);
//          //		printf("myVRCdata.mode= %d\r\n",myVRCdata.mode);
//          //		printf("myVRCdata.Channel_07= %d\r\n",myVRCdata.Channel_07);
//          //		printf("myVRCdata.Channel_08= %d\r\n",myVRCdata.Channel_08);
//          //		printf("myVRCdata.Channel_09= %d\r\n",myVRCdata.Channel_09);
//          //		printf("myVRCdata.Channel_10=%d\r\n",myVRCdata.Channel_10);
//          //		printf("myVRCdata.Channel_11=%d\r\n",myVRCdata.Channel_11);
//          //		printf("myVRCdata.Channel_12= %d\r\n",myVRCdata.Channel_12);
//          //		printf("myVRCdata.Channel_13= %d\r\n",myVRCdata.Channel_13);
//          //		printf("myVRCdata.Channel_14=  %d\r\n",myVRCdata.Channel_14);
//          //		printf("myVRCdata.Channel_15=  %d\r\n",myVRCdata.Channel_15);
//          myVRCdata.throttle=1500; //why? //yanly
//          //			printf("myVRCdata.throttle= %d \r\n",myVRCdata.throttle);
////          virtualrc.sendData(myVRCdata); //为什么要发送vrc命令？//by yanly
//          //			 printf("777777777777777777777777!\r\n");
//          //			coreApi->getReleaseControlMobileCMD();
//          //                mobileCommandHandler(coreApi, &flight);
//          //			coreApi->getReleaseControlMobileCMD_dji();
//          //		s=1;
//        }
//        if((tempture0<0)&&(tempture1<0))
//          s=1;
//        if (MAVLINK_TX_INIT_VAL - TimingDelay >= 4000)
//        {
//          dji_zkrt_read_heart_tempture_check();	/*将温度传感器数据填充到心跳包里面，并判断数据是否正常*/
//        }
//      }

//      if ((_read_count%10) == 0)
//      {

//        dji_bat_value_roll(); /*获取智能电池数据，并填充到智能电池数据数组中*/
//        dji_zkrt_read_heart_vol(); /*获取板载电压电流值*/

//        if (MAVLINK_TX_INIT_VAL - TimingDelay >= 4000)
//        {
//          dji_zkrt_read_heart_vol_check();	/*对当前板载电压电流值进行判断，并作出相应操作，将结果填充到心跳包*/
//        }
//      }
//      _read_count++;

//    }

//    if (MAVLINK_TX_INIT_VAL - TimingDelay >= 5000)
//    {
//      if ((mavlink_send_flag-TimingDelay) < 50)
//      {
//        GPIO_ResetBits(GPIOC, GPIO_Pin_0);
//      }
//      else if ((mavlink_send_flag-TimingDelay) < 800)
//      {
//        GPIO_SetBits(GPIOC, GPIO_Pin_0);
//      }
//      else if ((mavlink_send_flag-TimingDelay) >= 800)  //800ms
//      {
//        mavlink_send_flag = TimingDelay;
//#if 1
//        switch (mavlink_type_flag_dji)
//        {
//        case 0:
//          mavlink_type_flag_dji = 1;
//          dji_zkrt_read_heart_ack(); /*完成最终的数据包，并且发送出处*/
//          coreApi->sendPoll();
//          break;
//        case 1:
//          mavlink_type_flag_dji = 0;
//          dji_bat_value_send();  /*发送智能电池数据，总共32个字节*/    //????为什么没有封装zkrt packet格式
//          coreApi->sendPoll();
//          break;
//        }
//#endif
//        printf("T1 = %d T2 = %d  ADC1_25_dji=%d  ADC1_5_dji=%d  ADC1_I_dji=%d   LOW_VALUE = %d HIGH_VALUE = %d STA1 = %x STA2 = %x\r\n",
//               tempture0, tempture1, ADC1_25_dji,ADC1_5_dji,ADC1_I_dji,glo_tempture_low, glo_tempture_high, msg_smartbat_dji_buffer[0],
//               msg_smartbat_dji_buffer[3]);
//      }
//    }

//    if ((usart1_tx_flag-TimingDelay) >= 50)
//    {
//      GPIO_SetBits(GPIOD, GPIO_Pin_0);
//    }

//    if ((usart1_rx_flag-TimingDelay) >= 50)
//    {
//      GPIO_SetBits(GPIOD, GPIO_Pin_1);
//    }

//    if ((can_rx_flag-TimingDelay)  >= 50)
//    {
//      GPIO_SetBits(GPIOD, GPIO_Pin_4);
//    }

//    if ((can_tx_flag-TimingDelay)  >= 50)
//    {
//      GPIO_SetBits(GPIOD, GPIO_Pin_5);
//    }

//    if ((posion_recv_flag-TimingDelay)  >= 5000)
//    {
//      memset((void *)(msg_smartbat_dji_buffer+10), 0, 13);
//      msg_smartbat_dji_buffer[23] &= 0XEF;
//    }

//    if ((throw_recv_flag-TimingDelay)  >= 5000)
//    {
//      msg_smartbat_dji_buffer[23] &= 0XDF;
//    }
//    if ((camera_recv_flag-TimingDelay) >= 5000)
//    {
//      msg_smartbat_dji_buffer[23] &= 0XF7;
//    }
//    if ((flash_buffer._tempture_low != glo_tempture_low)||(flash_buffer._tempture_high != glo_tempture_high))//在这里将所有的值统一更新，否则flash写入太占时间可能导致写入错误
//    {
//      flash_buffer._tempture_low  = glo_tempture_low;				//步骤二：保存该变量到缓存里
//      flash_buffer._tempture_high = glo_tempture_high;
//      STMFLASH_Write();											//步骤三：编写flash
//    }
//    IWDG_Feed();
//  }
//}

///**
//  * @brief  DJI init. 放在硬件初始化完成之后
//  * @param  None
//  * @retval None
//  */
//void dji_init()
//{
//  ZKRT_LOG(LOG_NOTICE,"This is the example App to test DJI onboard SDK on STM32F4Discovery Board! \r\n");
//  ZKRT_LOG(LOG_NOTICE,"Refer to \r\n");
//  ZKRT_LOG(LOG_NOTICE,"https://developer.dji.com/onboard-sdk/documentation/github-platform-docs/STM32/README.html \r\n");
//  ZKRT_LOG(LOG_NOTICE,"for supported commands!\r\n");
//  ZKRT_LOG(LOG_NOTICE,"Board Initialization Done!\r\n");
//  //! Change the version string to your platform/version as defined in DJI_Version.h
//  coreApi->setVersion(versionA3_31);
//  delay_nms(200);
//  ZKRT_LOG(LOG_NOTICE,"API Version Set Done!\r\n");	
//}
///**
//  * @brief  dji_process. 
//  * @param  None
//  * @retval None
//  */
//void dji_process()
//{
//	// One time automatic activation
//	if(runOnce)
//	{
//		runOnce = 0;
//		coreApi->setBroadcastFreq(myFreq);
//		delay_nms(50);

//		// The Local navigation example will run in broadcast call back function,
//		// immediate after GPS position is updated
//		coreApi->setBroadcastCallback(myRecvCallback,(DJI::UserData)(&droneState));

//		User_Activate();
//		delay_nms(50);

//		next500MilTick = driver->getTimeStamp() + 500;
//	}

//	//   virtualrc.sendData(myVRCdata);
//	// virtualrc.setControl(0,virtualrc.CutOff_ToLogic);

//	if(driver->getTimeStamp() >= next500MilTick) 		//暂时不会跑到这里，dji用的定时器没有初始化。	//note by yanly 
//	{
//		next500MilTick = driver->getTimeStamp() + 500;

//		// Handle user commands from mobile device
//		mobileCommandHandler(coreApi, &flight);

//		// Handle user commands from serial (USART2)
//		//   myTerminal.terminalCommandHandler(coreApi, &flight);
//	}
//	
//	coreApi->sendPoll();	
//}
///**
//  * @brief  tempture_flight_control. 温度控制飞行+温度信息组包到心跳包里
//  * @param  None
//  * @retval None
//  */
//void tempture_flight_control(void)
//{
//	if (_160_read_flag - TimingDelay >= 160)
//	{
//		_160_read_flag = TimingDelay;
//		ADC_SoftwareStartConv(ADC1); /*启动ADC*/	
////		ZKRT_LOG(LOG_NOTICE, "ADC_SoftwareStartConv!\r\n");	
//		if((_read_count%2) == 0)									
//		{
//			zkrt_dji_read_heart_tempture();  /*获取温度传感器数据*/
//			if(((tempture0>0)||(tempture1>0))&&(tempture_invalid==0))
//			{
//				virtualrc.setControl(1,virtualrc.CutOff_ToRealRC);
//				myVRCdata=virtualrc.getVRCData();
//				myVRCdata.throttle=1500;
//				virtualrc.sendData(myVRCdata);
//			}
//			if((tempture0<0)&&(tempture1<0))
//			{
//				tempture_invalid=1;				
//			}
//			if (MAVLINK_TX_INIT_VAL - TimingDelay >= 4000)	
//			{					
//				dji_zkrt_read_heart_tempture_check();	/*将温度传感器数据填充到心跳包里面，并判断数据是否正常*/	
//			}
//		}
//		if ((_read_count%10) == 0)								
//		{
//			dji_bat_value_roll(); /*获取智能电池数据，并填充到智能电池数据数组中*/
//			dji_zkrt_read_heart_vol(); /*获取板载电压电流值*/

//			if (MAVLINK_TX_INIT_VAL - TimingDelay >= 4000)	
//			{
//				dji_zkrt_read_heart_vol_check();	/*对当前板载电压电流值进行判断，并作出相应操作，将结果填充到心跳包*/					
//			}
//		}
//		_read_count++;
//	}	
//}
///**
//  * @brief  mobile_heardbeat_packet_control. 板子与地面站（手机）心跳包处理（定时发送）
//  * @param  None
//  * @retval None
//  */
//void mobile_heardbeat_packet_control(void)
//{
//	if (MAVLINK_TX_INIT_VAL - TimingDelay >= 5000)  //5s延时到之后一直处理以下逻辑
//	{
//		if ((mavlink_send_flag-TimingDelay) >= 800)
//		{
//			mavlink_send_flag = TimingDelay;
//			#if 1
//			switch (mavlink_type_flag_dji)
//			{
//				case 0:
//					mavlink_type_flag_dji = 1;
//					dji_zkrt_read_heart_ack(); /*完成最终的数据包，并且发送出处*/
//					coreApi->sendPoll();
//					break;
//				case 1:
//					mavlink_type_flag_dji = 0;
//				//dji_bat_value_send();  /*发送智能电池数据，总共32个字节*/    //????为什么没有封装zkrt packet格式 //modify by yanly
//					coreApi->sendPoll();
//					break;
//				default:
//					break;
//			}
//			#endif
////			printf("T1 = %d T2 = %d  ADC1_25_dji=%d  ADC1_5_dji=%d  ADC1_I_dji=%d   LOW_VALUE = %d HIGH_VALUE = %d STA1 = %x STA2 = %x\r\n",
////			tempture0, tempture1, ADC1_25_dji,ADC1_5_dji,ADC1_I_dji,glo_tempture_low, glo_tempture_high, msg_smartbat_dji_buffer[0],
////			msg_smartbat_dji_buffer[3]);
//		}
//	}	
//}
///**
//  * @brief  product_main. 正式产品主函数
//  * @param  None
//  * @retval None
//  */
//int product_main()
//{
//  BSPinit();
////  delay_nms(20000);
//	ZKRT_LOG(LOG_NOTICE, "==================================================\r\n"); 
//	printf("PRODUCT_NAME: %s\r\nPRODUCT_ID: %s\r\nPRODUCT_VERSION: %s\r\nPRODUCT_TIME: %s %s\r\n",PRODUCT_NAME,PRODUCT_ID,PRODUCT_VERSION,__DATE__,__TIME__);
//	ZKRT_LOG(LOG_NOTICE, "==================================================\r\n"); 
//#ifdef USE_DJI_FUN	
//	dji_init();
//#endif
//#ifdef USE_LWIP_FUN	
//	lwip_prcs_init();
//#endif	
//  while (1)
//  {
//#ifdef USE_DJI_FUN			
//		dji_process();                        //大疆SDK处理
//#endif		
//		mobile_data_process();                //将接收到的mobile透传数据进行解析处理
//    main_zkrt_dji_recv();		              //从子模块通过CAN接收数据，将毒气、抛投数据填充到心跳包，并且相关位置一
//		tempture_flight_control();            //温度超过上下限启动逃逸功能
//		mobile_heardbeat_packet_control();    //板子定时发送心跳包到地面站
//		led_process();                        //LED控制
//#ifdef USE_LWIP_FUN			
//		lwip_prcs();													//网络流程
//#endif
//#ifdef USE_USB_FUN			
//		usb_user_prcs();                      //USB流程
//#endif    
//		IWDG_Feed();
//  }
//}
