/**
  -----------------------------------------------------------------------------
  * @file   : lcd_config.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/03
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
  * @description
  *          LCD屏的基本配置  定义屏幕的基本信息
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/
#ifndef __LCD_CONFIG_H__
#define __LCD_CONFIG_H__


typedef enum
{
  DISABLE = 0, 
  ENABLE = !DISABLE
}eFunctionState;

/*2英寸LCD 屏幕的基本信息*/
#define LCD_320X240      1    //屏幕名称
#define SCREEN_WIDTH     240 //屏幕行数量
#define SCREEN_HEIGHT    320 //屏幕列数量

#endif

