/**
  -----------------------------------------------------------------------------
  * @file   : lcd_driver.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/03
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
  * @description
  *              
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/ 
#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

#include <stdint.h>
#include "lcd_config.h"

//使用wiringPi库定义的GPIO引脚序号
#define LCD_RES_PIN 0  //复位引脚
#define LCD_DC_PIN  1  //寄存器/数据选择引脚
#define LCD_CS_PIN  10  //片选引脚 使用通道0
#define LCD_BK_PIN  2  //背光控制引脚


typedef enum
{
  command_mode = 0, 
  data_mode = 1
}eWriteMode;

typedef enum
{
  direction0,    //默认0度
  direction90,   //旋转90度
  direction180,  //旋转180度
  direction270   //旋转270度
}eLCDdirection;


//LCD参数结构体
typedef struct
{
  unsigned short int width;        //LCD显示区宽度
  unsigned short int height;       //LCD显示区高度
  unsigned short int id;           //LCD ID编码
  unsigned short int direction;    //显示方向指令，0-竖屏，1-横屏
  unsigned short int write_gram_cmd; //开始写GRAM指令
  unsigned short int set_x_cmd;      //设置X坐标指令
  unsigned short int set_y_cmd;      //设置Y坐标指令
}tLCDdevice;

void lcd_lowlevel_init(void);
void lcd_reset_chip(void);
void lcd_enable_chip(eFunctionState new_state);
void lcd_select_write_mode(eWriteMode mode);
void lcd_ctr_backlight(eFunctionState new_state);
void lcd_write_data8bit(uint8_t data);
void lcd_write_data16bit(uint16_t data);
void lcd_select_register(uint8_t reg);
void lcd_write_gram_ready(void);
void lcd_set_parameters(eLCDdirection dir);
int lcd_access_gram_region(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
int lcd_write_gram_region(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t color);
void lcd_set_cursor(uint16_t x, uint16_t y);




#endif
