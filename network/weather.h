/**
  -----------------------------------------------------------------------------
  * @file   : weather.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/20
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
#ifndef __WEATHER_H
#define __WEATHER_H

/* Includes ------------------------------------------------------------------*/
#include <pthread.h>

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

typedef struct
{
	/* 实况天气数据 */
	char id[32];				    //id
	char name[32];				  //地名
	char country[32];			  //国家
	char path[32];				  //完整地名路径
	char timezone[32];			//时区
	char timezone_offset[32];   //时差
	char text[32];				  //天气预报文字
	char code[32];				  //天气预报代码
	char temperature[32];   //气温
	char last_update[32];		//最后一次更新的时间
}tWeather;


typedef struct
{
	/* 获取天气信息的配置 */
	char id[5];				    //id
	char update_per[5];		//本地更新间隔 单位(分钟)
	char location[32];		  //地名
	char country[32];			  //国家
	char weather_json[12];	//获取天气信息类型 比如"now"
}tConfig;


int get_weather_info_start(pthread_t *tid);
void *pthread_get_weather(void *arg);



#endif /* __XXX_H */


/*---------------------------------The End------------------------------------*/
