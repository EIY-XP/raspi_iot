/**
  -----------------------------------------------------------------------------
  * @file   : lcd_basic.c
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

/* Includes ------------------------------------------------------------------*/
#include "lcd_basic.h"
#include "lcd_driver.h"
#include <string.h>



/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static tFont *current_font = &asc_font8x16_t;    // 默认字体为8x16的ASCII字符集
static tBmp *current_icon = &bmp50x60_raspberry_t;

static uint16_t current_text_color   = COLOR_BLACK;  // 默认为黑色字体
static uint16_t current_back_color   = COLOR_WHITE;  // 默认为白色字体
/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/


/**
  * @function : lcd_set_color
  * @author   : xp
  * @brief    : 设置字体颜色和背景颜色
  * @param    : 
  * @retval   : 
  */
void lcd_set_color(uint16_t text_color, uint16_t back_color)
{
  current_text_color = text_color;
  current_back_color = back_color;
}


/**
  * @function : lcd_get_color
  * @author   : xp
  * @brief    : 获取当前字体和背景颜色
  * @param    : 
  * @retval   : 
  */
void lcd_get_color(uint16_t *text_color, uint16_t *back_color)
{
  *text_color = current_text_color;
  *back_color = current_back_color;
}


/**
  * @function : lcd_set_text_color
  * @author   : xp
  * @brief    : 设置字体颜色
  * @param    : 
  * @retval   : 
  */
void lcd_set_text_color(uint16_t color)
{
  current_text_color = color;
}


 /**
  * @function : lcd_set_back_color
  * @author   : xp
  * @brief    : 设置背景颜色
  * @param    : 
  * @retval   : 
  */
void lcd_set_back_color(uint16_t color)
{
  current_back_color = color;
}


/**
  * @function : lcd_get_text_color
  * @author   : xp
  * @brief    : 获取字体颜色
  * @param    : 
  * @retval   : 
  */
void lcd_get_text_color(uint16_t *text_color)
{
  *text_color = current_text_color;
}


 /**
  * @function : lcd_get_back_color
  * @author   : xp
  * @brief    : 获取背景颜色
  * @param    : 
  * @retval   : 
  */
void lcd_get_back_color( uint16_t *back_color)
{
  *back_color = current_back_color;
}


 /**
  * @function : lcd_set_font
  * @author   : xp
  * @brief    : 设置字体
  * @param    : 
  * @retval   : 
  */
void lcd_set_font(tFont *fonts)
{
  current_font = fonts;
}


 /**
  * @function : lcd_get_font
  * @author   : xp
  * @brief    : 获取当前字体
  * @param    : 
  * @retval   : 
  */
tFont* lcd_get_font(void)
{
  return current_font;
}


 /**
  * @function : lcd_set_icon
  * @author   : xp
  * @brief    : 设置图标
  * @param    : 
  * @retval   : 
  */
void lcd_set_icon(tBmp *icon)
{
  current_icon = icon;
}


 /**
  * @function : lcd_init
  * @author   : xp
  * @brief    : 屏幕初始化
  * @param    : 
  * @retval   : 
  */
void lcd_init(void)
{
  lcd_lowlevel_init();
  lcd_set_color(COLOR_BLUE, COLOR_WHITE);
  lcd_clear(current_back_color);
  lcd_set_font(current_font);
  lcd_set_icon(current_icon);
}


 /**
  * @function : lcd_clear
  * @author   : xp
  * @brief    : 清屏幕
  * @param    : 
  * @retval   : 
  */
void lcd_clear(uint16_t color)
{
  lcd_write_gram_region(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}


/**
  * @function : lcd_display_temp_icon
  * @author   : xp
  * @brief    : 显示摄氏度(℃)图标 大小为16x16
  * @param    : 
  * @retval   : 
  */
void lcd_display_temp_icon(uint16_t pos_x, uint16_t pos_y, uint8_t *c)
{
	uint16_t i, j, k;
	uint16_t end_x, end_y;
	uint8_t temp;

	end_x = pos_x + 16 - 1;
	end_y = pos_y + 16 - 1;
	lcd_access_gram_region(pos_x, pos_y, end_x, end_y);

	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 2; j++)
		{
			temp = *(c + i * 2 + j);
			for (k = 0; k < 8; k++)
			{
				if (temp & (0x80 >> k))
					lcd_write_data16bit(current_text_color);
				else
					lcd_write_data16bit(current_back_color);
			}
		}
	}
}





 /**
  * @function : lcd_display_icon
  * @author   : xp
  * @brief    : 绘画图片 图标
  * @param    : 
  * @retval   : 
  */
void lcd_display_icon(uint16_t pos_x, uint16_t pos_y, tBmp *bmp)
{
  uint16_t end_x, end_y;
  uint16_t pixel;  // 像素值
  uint32_t index, pixel_total;
  

  end_x = pos_x + bmp->width - 1;
  end_y = pos_y + bmp->height - 1;
  pixel_total = (bmp->width) * (bmp->height);  //像素总数量

  lcd_access_gram_region(pos_x, pos_y, end_x, end_y);

  for (index = 0; index < pixel_total; index++)
  {
    //每个像素在table中分成2个字节存储，低字节在前，需要重新组合成一个16bit数据
    pixel = (bmp->table[index*2+1] << 8) | (bmp->table[index*2]);
    lcd_write_data16bit(pixel);
  }
}


 /**
  * @function : lcd_draw_char8x16
  * @author   : xp
  * @brief    : 绘画一个ASCII字符 可以显示8x16大小的字符 该字符点阵存储在ASC16文件中
  * @param    : 
  * @retval   : 
  */
void lcd_draw_char8x16(uint16_t pos_x, uint16_t pos_y, uint8_t *c)
{
  uint16_t i, j;
  uint16_t end_x, end_y;
  uint16_t temp;

  end_x = pos_x + current_font->width - 1;
  end_y = pos_y + current_font->height - 1;
  lcd_access_gram_region(pos_x, pos_y, end_x, end_y);

  for (i = 0; i < current_font->height; i++)
  { 
    temp = *(c + i); //取像素值
    for (j = 0; j < current_font->width; j++)
    {
      if (temp & (0x80 >> j))
      {
        lcd_write_data16bit(current_text_color);
      }
      else
      {
        lcd_write_data16bit(current_back_color);
      }
    }
  }
}


 /**
  * @function : lcd_draw_char24x48
  * @author   : xp
  * @brief    : 绘画一个ASCII字符 可以显示24x48大小的字符 该字符点阵存储在ASC48文件中
  * @param    : 
  * @retval   : 
  */
void lcd_draw_char24x48(uint16_t pos_x, uint16_t pos_y, uint8_t *c)
{
  uint16_t i, j, k;
  uint16_t end_x, end_y;
  uint16_t temp;

  end_x = pos_x + current_font->width - 1;
  end_y = pos_y + current_font->height - 1;
  lcd_access_gram_region(pos_x, pos_y, end_x, end_y);

  for (i = 0; i < current_font->height; i++)
  { 
		for (j = 0; j < (current_font->width) / 8; j++)
		{
			temp = *(c + i * 3 + j);
			for (k = 0; k < 8; k++)
			{
				if (temp & (0x80 >> k))
					lcd_write_data16bit(current_text_color);
				else
					lcd_write_data16bit(current_back_color);
			}
		}
  }
}



 /**
  * @function : lcd_draw_chinese16x16
  * @author   : xp
  * @brief    : 绘画一个汉字 可以显示16x16大小的汉字 该字符点阵存储在HZK16文件中
  * @param    : 
  * @retval   : 
  */
void lcd_draw_chinese16x16(uint16_t pos_x, uint16_t pos_y, uint8_t *c)
{
	uint16_t i, j, k;
	uint16_t end_x, end_y;
	uint8_t temp;

	end_x = pos_x + current_font->width - 1;
	end_y = pos_y + current_font->height - 1;
	lcd_access_gram_region(pos_x, pos_y, end_x, end_y);

	for (i = 0; i < current_font->height; i++)
	{
		for (j = 0; j < (current_font->width) / 8; j++)
		{
			temp = *(c + i * 2 + j);
			for (k = 0; k < 8; k++)
			{
				if (temp & (0x80 >> k))
					lcd_write_data16bit(current_text_color);
				else
					lcd_write_data16bit(current_back_color);
			}
		}
	}
}


 /**
  * @function : lcd_draw_char16x24
  * @author   : xp
  * @brief    : 绘画一个字符 可以显示16x24大小的字符 字符点阵存放在lcd_font.cpp中
  *             由于字模存储方式为16bit,实现方式有冲突 暂时不显示16x24大小的字符
  *             该函数本身没问题
  * @param    : 
  * @retval   : 
  */
void lcd_draw_char16x24(uint16_t pos_x, uint16_t pos_y, uint16_t *c)
{
  uint32_t index = 0, counter = 0;
  uint16_t end_x, end_y;
  uint16_t temp;

  end_x = pos_x + current_font->width - 1;
  end_y = pos_y + current_font->height - 1;
  lcd_access_gram_region(pos_x, pos_y, end_x, end_y);

  for (index = 0; index < current_font->height; index++)
  { 
    temp = *(c + index);   //取像素值
    
    for(counter = 0; counter < current_font->width; counter++)
    {
      if (temp & 0x01)
      {
        lcd_write_data16bit(current_text_color);
      }
      else
      {
        lcd_write_data16bit(current_back_color);
      }
      
     	temp >>= 1;
    }
  }
}



 /**
  * @function : lcd_display_char
  * @author   : xp
  * @brief    : 显示一个ASCII字符 ASCII16 ASCII48
  * @param    : 
  * @retval   : 
  */
void lcd_display_char(uint16_t pos_x, uint16_t pos_y, uint8_t c)
{	
	if ((current_font->font_type == asc) && (current_font->width == 8))
	{
		if (-1 == get_asc16_lattice(&c, current_font->table))
			return;
		lcd_draw_char8x16(pos_x, pos_y, current_font->table);
		memset(current_font->table, 0, current_font->buff_len);
	}
	else if ((current_font->font_type == asc) && (current_font->width == 24))
	{
		if (-1 == get_asc48_lattice(&c, current_font->table))
			return;
		lcd_draw_char24x48(pos_x, pos_y, current_font->table);
		memset(current_font->table, 0, current_font->buff_len);
	}
}


 /**
  * @function : lcd_display_srting
  * @author   : xp
  * @brief    : 显示ASCII字符串
  * @param    : 
  * @retval   : 
  */
void lcd_display_srting(uint16_t pos_x, uint16_t pos_y, uint8_t *str)
{
  uint16_t index = 0;
  uint8_t c;
  while (str[index] != '\0')
  {
    c = str[index];
    lcd_display_char(pos_x, pos_y, c);
    pos_x += current_font->width;
    index++;
  }
}


 /**
  * @function : lcd_display_chinese
  * @author   : xp
  * @brief    : 显示HZK16 汉字
  * @param    : 
  * @retval   : 
  */
void lcd_display_chinese(uint16_t pos_x, uint16_t pos_y, char *str)
{
	char out_buf[30];
	uint8_t i = 0;
	
	if (0 == unicode_to_gb2312(str, strlen(str), out_buf, sizeof(out_buf)))
	{
		do
		{
			if (-1 == get_hzk16_lattice(&out_buf[i], current_font->table))
				break;
				
			lcd_draw_chinese16x16(pos_x, pos_y, current_font->table);
			i += 2;
			pos_x += current_font->width;
		}while(out_buf[i] != '\0');
	}
}


 /**
  * @function : lcd_draw_point
  * @author   : xp
  * @brief    : 使用color颜色绘画一个点
  * @param    : 
  * @retval   : 
  */
void lcd_draw_point(uint16_t pos_x, uint16_t pos_y, uint16_t color)
{
  lcd_set_cursor(pos_x, pos_y);
  lcd_write_data16bit(color);
}


 /**
  * @function : lcd_draw_line
  * @author   : xp
  * @brief    : 绘画一条直线 线宽为一个像素点
  *						  本方法目前只实现了从左到右 和 从上到下 的方向绘画一条直线
  * @param    : pos_x X轴坐标 范围 0-240
  *             pos_y Y轴坐标 范围 0-320
  *             length 线的长度 结合方向 不要超过屏幕的尺寸范围
  *             draw_dir 方向 DIR_H(==0 水平方向) DIR_V(==1 垂直方向) 
  * @retval   : 
  */
void lcd_draw_line(uint16_t pos_x, uint16_t pos_y, uint16_t length, uint8_t draw_dir)
{
	uint16_t end_x, end_y;
	uint16_t i;
	
	switch (draw_dir)
	{
		case direction_h:  //水平方向
			if ((pos_x + length) >= SCREEN_WIDTH)
			{
				end_x = SCREEN_WIDTH - 1;
				length = SCREEN_WIDTH - pos_x;
			}
			else
				end_x = pos_x + length;

			end_y = pos_y;
			if (-1 == lcd_access_gram_region(pos_x, pos_y, end_x, end_y))
				return;
			
			for (i = 0; i < length; i++)
			{
				lcd_write_data16bit(current_text_color);
			}
			
			break;

		case direction_v:  //垂直方向
			if ((pos_y + length) >= SCREEN_HEIGHT)
			{
				end_y = SCREEN_HEIGHT - 1;
				length = SCREEN_HEIGHT - pos_y;
			}	
			else
				end_y = pos_y + length;

			end_x = pos_x;
			if (-1 == lcd_access_gram_region(pos_x, pos_y, end_x, end_y))
				return;
			
			for (i = 0; i < length; i++)
			{
				lcd_write_data16bit(current_text_color);
			}
			
			break;

		default:
			break;
	}
}


/**
  * @function : lcd_draw_Rect
  * @author   : xp
  * @brief    : 绘画一个矩形(不具备填充) 线宽为一个像素点
  * @param    : pos_x X轴坐标 范围 0-240
  *             pos_y Y轴坐标 范围 0-320
  *             width 宽度
  *             height 高度
  * @retval   : 
  */
void lcd_draw_Rect(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height)
{
	lcd_draw_line(pos_x, pos_y, width, direction_h);  //上横线
	lcd_draw_line(pos_x, pos_y, height, direction_v); //左竖线
	lcd_draw_line(pos_x, (pos_y+height), width, direction_h); //下横线
	lcd_draw_line((pos_x+width), pos_y, height, direction_v); //右竖线
}


/*---------------------------------The End------------------------------------*/

 
