/**
  -----------------------------------------------------------------------------
  * @file   : network.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/18
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2017 xxxx,All rights reserved  
  * @description
  *              
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NET_WORK_H
#define __NET_WORK_H

/* Includes ------------------------------------------------------------------*/
#include <pthread.h>

/* Exported macro ------------------------------------------------------------*/
#define IP_LENGTH   16
#define MAC_LENGTH  18

/* Exported constants --------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

int tcp_server_start(pthread_t *tid);
int get_local_ipv4_address(const char *eth_name, char *ip_addr);
int set_local_ipv4_address(const char *eth_name, const char *ip_addr);
int get_loacl_mac_address(const char *eth_name, char *mac_addr);
int set_local_mac_address(const char *eth_name, const char *mac_addr);
void *thread_tcp_server(void *arg);




#endif /* __NET_WORK_H */

/*---------------------------------The End------------------------------------*/
