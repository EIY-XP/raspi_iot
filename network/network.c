/**
  -----------------------------------------------------------------------------
  * @file   : network.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/18
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2017 xxxx,All rights reserved  
  * @description
  *            
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/
/* Includes ------------------------------------------------------------------*/
#include "network.h"
#include "../file/file.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/route.h>



/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

#define LOCAL_PORT 8080  //端口号
#define LOCAL_IP   "192.168.137.88"
char net_buf[512];       //收发buf

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/**
  * @function : tcp_server_start
  * @author   : xp
  * @brief    : 开启tcp服务
  * @param    : 
  *             
  * @retval   : 
  */
int tcp_server_start(pthread_t *tid)
{
	if (pthread_create(tid, NULL, (void*)thread_tcp_server, NULL))
    exit(-1);
  return 0;
}


/**
  * @function : get_local_ipv4_address
  * @author   : xp
  * @brief    : 获取指定网卡的IPv4地址
  * @param    : *eth_name 网卡
  *             *ip_addr 获取的IP存放buf
  * @retval   : 
  */
//int get_local_ipv4_address(const char *eth_name, char *ip_addr)
//{
//	int socket_fd;
//	struct sockaddr_in sin;
//	struct ifreq ifr;
//
//	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
//		return -1;
//
//	memset(&ifr, 0, sizeof(ifr));
//	strncpy(ifr.ifr_name, eth_name, IFNAMSIZ);
//	ifr.ifr_name[IFNAMSIZ - 1] = 0;
//
//	if (ioctl(socket_fd, SIOCGIFADDR, &ifr) < 0)
//	{
//		close(socket_fd);
//		return -1;
//	}
//
//	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
//	snprintf(ip_addr, IP_LENGTH, "%s", inet_ntoa(sin.sin_addr));//将地址格式转换成xxx.xxx.xxx.xxx
//
//	close(socket_fd);
//	return 0;
//}

int get_local_ipv4_address(const char *eth_name, char *ip_addr)
{
	if (!eth_name && !ip_addr)
		return -1;

	struct ifreq ifr;
	int socket_fd;

	if ((socket_fd = socket(AF_INET, SOCK_DGRAM ,0)) < 0)
		return -1;

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, eth_name, IF_NAMESIZE);
	struct sockaddr_in sin;
	if (ioctl(socket_fd, SIOCGIFADDR, &ifr) == -1)
	{
		close(socket_fd);
		return -1;
	}

	close(socket_fd);
	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	snprintf(ip_addr, IP_LENGTH, "%s", inet_ntoa(sin.sin_addr));
	return 0;
}




/**
  * @function : set_local_ipv4_address
  * @author   : xp
  * @brief    : 设置指定网卡的IPv4地址
  * @param    : *eth_name 网卡
  *             *ip_addr 设置的IP地址
  * @retval   : 未验证
  */
int set_local_ipv4_address(const char *eth_name, const char *ip_addr)
{
	if (!eth_name || !ip_addr)
		return -1;

	int socket_fd;
	struct ifreq ifr;

	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return -2;

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, eth_name, IF_NAMESIZE);
	struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
	sin->sin_family = AF_INET;

	if (inet_aton(ip_addr, &(sin->sin_addr)) < 0)
	{
		close(socket_fd);
		return -3;
	}

	int ret = ioctl(socket_fd, SIOCSIFADDR, &ifr);
	close(socket_fd);
	return (ret == -1) ? -4 : 0;
}



/**
  * @function : get_loacl_mac_address
  * @author   : xp
  * @brief    : 获取指定网卡的MAC地址
  * @param    : *eth_name 网卡
  *             *mac_addr 获取的mac存放buf
  * @retval   : 
  */
int get_loacl_mac_address(const char *eth_name, char *mac_addr)
{
	struct ifreq ifr;
	int socket_fd;

	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return -1;
	
//	memset(&ifr, 0 ,sizeof(ifr));
	bzero(&ifr, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, eth_name, sizeof(ifr.ifr_name)-1);

	if (ioctl(socket_fd, SIOCGIFHWADDR, &ifr) < 0)
	{
		close(socket_fd);
		return -1;
	}

	snprintf(mac_addr, MAC_LENGTH, "%02x:%02x:%02x:%02x:%02x:%02x",
					 (unsigned char)ifr.ifr_hwaddr.sa_data[0],
					 (unsigned char)ifr.ifr_hwaddr.sa_data[1],
					 (unsigned char)ifr.ifr_hwaddr.sa_data[2],
					 (unsigned char)ifr.ifr_hwaddr.sa_data[3],
					 (unsigned char)ifr.ifr_hwaddr.sa_data[4],
					 (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

	close(socket_fd);
	return 0;
}



/**
  * @function : set_local_mac_address
  * @author   : xp
  * @brief    : 设置指定网卡的MAC地址
  * @param    : *eth_name 网卡
  *             *mac_addr 设置的mac地址
  * @retval   : 未验证
  */
int set_local_mac_address(const char *eth_name, const char *mac_addr)
{
	if (!eth_name || !mac_addr)
		return -1;

	int socket_fd;
	struct ifreq ifr;

	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return -1;

	unsigned char temp[6];
	sscanf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
				 (unsigned int *)&temp[0],
				 (unsigned int *)&temp[1],
				 (unsigned int *)&temp[2],
				 (unsigned int *)&temp[3],
				 (unsigned int *)&temp[4],
				 (unsigned int *)&temp[5]);
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, eth_name, IF_NAMESIZE);
	struct sockaddr *addr = &ifr.ifr_hwaddr;
	addr->sa_family = ARPHRD_ETHER;
	memcpy(addr->sa_data, temp, 6);

	int ret = ioctl(socket_fd, SIOCSIFHWADDR, &ifr);
	close(socket_fd);
	return (ret == -1) ? -1 : 0;
}


/**
  * @function : thread_tcp_server
  * @author   : xp
  * @brief    : TCP server thread
  * @param    : 
  * @retval   : 
  */
void *thread_tcp_server(void *arg)
{
	int socket_fd;
	int new_conn;
	int rev_length;
	socklen_t length;

	struct sockaddr_in server_addr, client_addr;
	server_addr.sin_family = AF_INET;  //IPv4
	server_addr.sin_port = htons(LOCAL_PORT);
	server_addr.sin_addr.s_addr = inet_addr(LOCAL_IP);

	/*使用TCP通信 IPV4*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == socket_fd)
	{
		pthread_exit((void*)-1);
	}

	if (-1 == bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		pthread_exit((void*)-1);
	}

	if (-1 == listen(socket_fd, 5))
	{
		pthread_exit((void*)-1);
	}

	/*建立新连接*/
	length = sizeof(client_addr);
	new_conn = accept(socket_fd, (struct sockaddr*)&client_addr, &length);
	if (new_conn < 0)
	{
		pthread_exit((void*)-1);
	}

	while (1)
	{
		memset(net_buf, 0, sizeof(net_buf));
		rev_length = recv(new_conn, net_buf, sizeof(net_buf), 0);
		send(new_conn, net_buf, rev_length, 0);
	}

	close(new_conn);
	close(socket_fd);
	
	pthread_exit(NULL);
}
 
/*---------------------------------The End------------------------------------*/
