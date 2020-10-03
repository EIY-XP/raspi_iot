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


int get_asc16_lattice(unsigned char *c, unsigned char *buffer);
int get_asc48_lattice(unsigned char *c, unsigned char *buffer);
int get_hzk16_lattice(char *c, unsigned char *buffer);
void print_asc16_lattice(unsigned char *mat, unsigned char *solid_dot, unsigned char *hollow_dot);
void print_asc48_lattice(unsigned char *mat, unsigned char *solid_dot, unsigned char *hollow_dot);
void print_hzk16_lattice(unsigned char *mat, unsigned char *solid_dot, unsigned char *hollow_dot);
int code_convert(char *from_charset, char *to_charset, char *in_buf, unsigned int in_len, char *out_buf, unsigned int out_len);
int unicode_to_gb2312(char *in_buf, unsigned int in_len, char *out_buf, unsigned int out_len);

#endif 