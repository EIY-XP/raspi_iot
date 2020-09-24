/**
  -----------------------------------------------------------------------------
  * @file   : lcd_fonts.h
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

#ifndef __LCD_FONTS_H__
#define __LCD_FONTS_H__

#define LATTICE_TABLE_LEN 144

typedef enum
{
	asc = 0,  //ascii字符
	hzk = 1   //HZK字符
}eFontForm;

typedef struct
{
  const unsigned char* table;
  unsigned short int width;
  unsigned short int height;
}tBmp;


typedef struct
{
	eFontForm font_type;           //字符类型 ASCII HZK
  unsigned char* table;          //字符点阵临时存储缓冲区
  unsigned short int buff_len; //table缓冲区长度
  unsigned short int width;      //字符宽度
  unsigned short int height;   //字符高度
}tFont;


extern unsigned char bmp16x16_tem_table[32];

extern tFont asc_font8x16_t;
extern tFont asc_font24x48_t;
extern tFont hzk_font16x16_t;
extern tBmp bmp50x60_raspberry_t;

extern tBmp weather_code0_bmp51x51_t;
extern tBmp weather_code1_bmp51x52_t;
extern tBmp weather_code4_bmp60x47_t;
extern tBmp weather_code5_bmp60x44_t;
extern tBmp weather_code6_bmp60x51_t;
extern tBmp weather_code7_bmp60x42_t;
extern tBmp weather_code8_bmp56x49_t;
extern tBmp weather_code9_bmp60x40_t;
extern tBmp weather_code10_bmp60x59_t;
extern tBmp weather_code11_bmp56x56_t;
extern tBmp weather_code12_bmp56x56_t;
extern tBmp weather_code13_bmp56x54_t;
extern tBmp weather_code14_bmp56x54_t;
extern tBmp weather_code15_bmp56x54_t;
extern tBmp weather_code16_bmp56x54_t;
extern tBmp weather_code17_bmp56x54_t;
extern tBmp weather_code18_bmp57x54_t;
extern tBmp weather_code99_bmp53x23_t;





int get_asc16_lattice(unsigned char *c, unsigned char *buffer);
int get_asc48_lattice(unsigned char *c, unsigned char *buffer);
int get_hzk16_lattice(char *c, unsigned char *buffer);
void print_asc16_lattice(unsigned char *mat, unsigned char *solid_dot, unsigned char *hollow_dot);
void print_asc48_lattice(unsigned char *mat, unsigned char *solid_dot, unsigned char *hollow_dot);
void print_hzk16_lattice(unsigned char *mat, unsigned char *solid_dot, unsigned char *hollow_dot);
int code_convert(char *from_charset, char *to_charset, char *in_buf, unsigned int in_len, char *out_buf, unsigned int out_len);
int unicode_to_gb2312(char *in_buf, unsigned int in_len, char *out_buf, unsigned int out_len);

#endif 