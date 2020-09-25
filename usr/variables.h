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
#include "../network/weather.h"

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/* Exported variables ------------------------------------------------------- */


extern sem_t sem_sensor;
extern double sensor_temp; 
extern double sensor_hum;

extern tWeather weather_data;
extern pthread_mutex_t weather_mutex;


#endif




