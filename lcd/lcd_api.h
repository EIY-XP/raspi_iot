/**
  -----------------------------------------------------------------------------
  * @file   : lcd_api.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/03
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
  * @description
  *          LCD对外可使用的API函数说明 
  *          LCD说明：
  *          使用的2.0英寸ISP显示屏(型号为MSP2008)，内部驱动IC为ST7789，分辨率为320x240
  *          RGB565
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/
#ifndef __LCD_API_H__
#define __LCD_API_H__


#include <stdint.h>
#include "lcd_fonts.h"
#include "lcd_config.h"


//lcd_basic.c
void lcd_set_color(uint16_t text_color, uint16_t back_color);
void lcd_get_color(uint16_t *text_color, uint16_t *back_color);
void lcd_set_text_color(uint16_t color);
void lcd_set_back_color(uint16_t color);
void lcd_set_font(tFont *fonts);
tFont* lcd_get_font(void);
void lcd_set_icon(tBmp *icon);
void lcd_init(void);
void lcd_clear(uint16_t color);

void lcd_display_icon(uint16_t pos_x, uint16_t pos_y, tBmp *bmp);
void lcd_draw_char8x16(uint16_t pos_x, uint16_t pos_y, uint8_t *c);
void lcd_draw_char24x48(uint16_t pos_x, uint16_t pos_y, uint8_t *c);
void lcd_draw_chinese16x16(uint16_t pos_x, uint16_t pos_y, uint8_t *c);
void lcd_draw_char16x24(uint16_t pos_x, uint16_t pos_y, uint16_t *c);
void lcd_display_char(uint16_t pos_x, uint16_t pos_y, uint8_t c);
void lcd_display_srting(uint16_t pos_x, uint16_t pos_y, uint8_t *str);
void lcd_display_chinese(uint16_t pos_x, uint16_t pos_y, char *str);
void lcd_draw_point(uint16_t pos_x, uint16_t pos_y, uint16_t color);
void lcd_draw_line(uint16_t pos_x, uint16_t pos_y, uint16_t length, uint8_t draw_dir);
void lcd_draw_Rect(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height);

//lcd_driver
void lcd_backlight_ctr(eFunctionState new_state);
int lcd_access_gram_region(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
int lcd_write_gram_region(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t color);
void lcd_set_cursor(uint16_t x, uint16_t y);



#endif

