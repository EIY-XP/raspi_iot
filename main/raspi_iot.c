/**
  -----------------------------------------------------------------------------
  * @file   : main.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/08/25
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
  * @description
  *           raspberryPi monitor 将raspberryPi的系统信息显示在OLED显示屏上
  *           显示内容包括但不限于 开机画面 系统CPU温度 系统网络信息 系统日历等 
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/

/* Includes ------------------------------------------------------------------*/
#include "../usr/includes.h"

#include "../picture/bmp.h"

//wiringPi库函数
#include <wiringPi.h>

#include <stdint.h>
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
int raspi_iot_init(void);

/**
  * @function : main
  * @author   : xp
  * @brief    : 主函数
  * @param    : 
  * @retval   : 
  */
int main()
{
	pthread_t tid_lcd;
	pthread_t tid_modbus;
	pthread_t tid_weather;
	pthread_t tid_tcp_server;
	void *thread_ret = NULL;
	
	if (raspi_iot_init() == -1)
		exit(-1);


	rtu_modbus_poll_start(&tid_modbus);
	get_weather_info_start(&tid_weather);
	display_device_info_start(&tid_lcd);
	tcp_server_start(&tid_tcp_server);
	
	pthread_detach(tid_lcd);
	pthread_detach(tid_modbus);
	pthread_detach(tid_weather);
	pthread_join(tid_tcp_server, &thread_ret);
	if (*(int*)thread_ret == -1)  //线程异常退出
		exit(-1);
	
	while (1)
	{
		
	}

	exit(0);
}


/**
  * @function : raspi_iot_init
  * @author   : xp
  * @brief    : 应用初始化
  * @param    : 
  * @retval   : 
  */
int raspi_iot_init(void)
{
	/*
	使用wiringPi库之前，必须现在树莓派上安装该库，然后初始化
	该库安装完成后，头文件已存放在/usr/local/include和/usr/include两个路径中
	*/
	if ((wiringPiSetup()) < 0)
		return -1;
	
	return 0;
}

/*---------------------------------The End------------------------------------*/


