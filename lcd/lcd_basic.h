/**
  -----------------------------------------------------------------------------
  * @file   : lcd_basic.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/03
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
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
#ifndef __LCD_BASIC_H__
#define __LCD_BASIC_H__

#include <stdint.h>
#include "lcd_fonts.h"
#include "lcd_config.h"

#define COLOR_WHITE     0xFFFF  //白色
#define COLOR_BLACK     0x0000  //黑色   
#define COLOR_NAVY      0x000F  //深蓝色  
#define COLOR_DGREEN    0x03E0  //深绿色  
#define COLOR_DCYAN     0x03EF  //深青色  
#define COLOR_MAROON    0x7800  //深红色      
#define COLOR_PURPLE    0x780F  //紫色  
#define COLOR_OLIVE     0x7BE0  //橄榄绿 

#define COLOR_GREY      0xF7DE     
#define COLOR_LGRAY     0xC618  //灰白色
#define COLOR_DGRAY     0x7BEF  //深灰色  

#define COLOR_BLUE      0x001F  //蓝色    
#define COLOR_BLUE2     0x051F

#define COLOR_GREEN     0x07E0  //绿色          
#define COLOR_CYAN      0x07FF  //青色  
#define COLOR_CYAN2      0x7FFF  //蓝绿色

#define COLOR_RED       0xF800  //红色       
#define COLOR_MAGENTA   0xF81F  //品红    

#define COLOR_YELLOW    0xFFE0  //黄色        
#define COLOR_ORANGE    0xE301  //橘黄色

#define COLOR_CROWGREEN 0x2986  //藏青色

typedef enum
{
	direction_h = 0, //绘画水平方向
	direction_v = 1 //绘画垂直方向
}eDrawDirection;


void lcd_set_color(uint16_t text_color, uint16_t back_color);
void lcd_get_color(uint16_t *text_color, uint16_t *back_color);
void lcd_set_text_color(uint16_t color);
void lcd_set_back_color(uint16_t color);
void lcd_get_text_color(uint16_t *text_color);
void lcd_get_back_color( uint16_t *back_color);
void lcd_set_font(tFont *fonts);
tFont* lcd_get_font(void);
void lcd_set_icon(tBmp *icon);
void lcd_init(void);
void lcd_clear(uint16_t color);

void lcd_display_temp_icon(uint16_t pos_x, uint16_t pos_y, uint8_t *c);
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


#endif