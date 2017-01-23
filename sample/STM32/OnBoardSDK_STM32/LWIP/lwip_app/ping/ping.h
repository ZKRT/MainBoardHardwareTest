#ifndef __PING_H__
#define __PING_H__

#include "lwip/ip_addr.h"

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif

/* user defines ------------------------------------------------------------------*/
/** ping target - should be a "ip_addr_t" */
#define PING_TARGET	   {IPADDR_ANY}  //使用本地IP地址 ping自身
//#define PING_TARGET	   {0x9c01a8c0} 
#define PING_DELAY     1000

typedef struct
{
	ip_addr_t ping_target;
	long long s_ping_req_cnt;     //发送ping次数
	long long r_ping_reply_cnt;   //接收ping回复次数
	u32_t *ping_time;
}ping_devsend_config_st;


void ping_init(void);
#if !PING_USE_SOCKETS
void ping_send_now(void);
#endif /* !PING_USE_SOCKETS */
void ping_prcs(void);

extern ping_devsend_config_st my_ping_config;

#endif /* __PING_H__ */
