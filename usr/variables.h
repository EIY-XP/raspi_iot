/**
  -----------------------------------------------------------------------------
  * @file   : variables.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/13
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2017 xxxx,All rights reserved  
  * @description
  *            变量数据头文件
  *            全局变量数据在此文件中声明
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VARIABLES_H
#define __VARIABLES_H

/* Includes ------------------------------------------------------------------*/
#include <semaphore.h>  //信号量

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/* Exported variables ------------------------------------------------------- */


extern double cpu_temp;

extern char date_str[20];
extern char time_str[10];
extern unsigned char week_day;


extern sem_t sem_sensor;
extern double sensor_temp; 
extern double sensor_hum;

extern char eth0_ip[16];
extern char wlan0_ip[16];


#endif




