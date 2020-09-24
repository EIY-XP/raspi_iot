/**
  -----------------------------------------------------------------------------
  * @file   : delay.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/03
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
  * @description
  *            基于linux延时函数
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/

/* Includes ------------------------------------------------------------------*/
#include "delay.h"

#include <time.h>
#include <unistd.h>


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/**
  * @function : delay_msec
  * @author   : xp
  * @brief    : 毫秒级延时 最小单位1毫秒 该延时会交出CPU使用权
  * @param    : 
  * @retval   : 
  */
void delay_msec(unsigned int how_long)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(how_long / 1000) ;
  sleeper.tv_nsec = (long)(how_long % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}


/**
  * @function : delay_sec
  * @author   : xp
  * @brief    : 秒级延时 最小单位1秒 该延时会交出CPU使用权
  * @param    : 
  * @retval   : 
  */
void delay_sec(unsigned int how_long)
{
	sleep(how_long);
}




