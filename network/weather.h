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


int get_weather(char *weather_json, char *location, tWeather *result);
void display_weather_test(void);









#endif /* __XXX_H */


/*---------------------------------The End------------------------------------*/
