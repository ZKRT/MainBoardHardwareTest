/*! @file Activate.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Activation process for the STM32 example App.
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#include "Activate.h"

extern CoreAPI defaultAPI;
extern CoreAPI *coreApi;
extern Flight flight;
extern FlightData flightData;

void
User_Activate ()
{
	printf("@@@@@@@@User_Activate ()!@@@@@@@@@\n");
	static char key_buf[65] = "ac5100a048cc4c4a08ce23200481d058d7455e69b8668fc0e49f29bd856c185a"; /*"your app_key"*/

	ActivateData user_act_data;
	user_act_data.ID = 1031967; /*need your key in number like: 111000*/


	//! Change the version string to your platform/version as defined in DJI_Version.h
	user_act_data.version = versionA3_31;

	user_act_data.encKey = key_buf;

	coreApi->activate (&user_act_data);

}
