/**
  -----------------------------------------------------------------------------
  * @file   : bmp.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/28
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
#ifndef __BMP_H
#define __BMP_H

/* Includes ------------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/*BMP图片文件头结构体 一共占用14个字节(0~13)*/
typedef struct
{
	unsigned char   bfType[2];     //(2字节)文件类型 必须以BM开头
	unsigned long   bfSize;        //(4字节)位图大小
	unsigned short bfReserved1;    //(2字节)保留
	unsigned short bfReserved2;    //(2字节)保留
	unsigned long   bfOffBit;      //(4字节)到位图数据偏移量
}tBitMapFileHeader;


/*BMP图片信息头结构体 一共占用40个字节*/
typedef struct
{
	unsigned long biSize;     //(4字节)BitMapFileHeader字节数
	long biWidth;             //(4字节)图像的宽度 用像素为单位
	long biHeight;            //(4字节)图像的高度 用像素为单位
	unsigned short biPlanes;       //(2字节)目标设备说明颜色平面数
	unsigned short biBitCount;     //(2字节)比特数/像素 为1 4 8 16 24 32
	unsigned long biCompression;   //(4字节)图像数据压缩类型 没有压缩的类型：BI_RGB
	unsigned long biSizeImage;     //(4字节)图像的大小 以字节为单位
	long biXPelsPerMeter;          //(4字节)水平分辨率
	long biYPelsPerMeter;          //(4字节)垂直分辨率
	unsigned long biClrUsed;       //(4字节)位图实际使用的彩色表中的颜色索引数
	unsigned long biClrImportant;  //(4字节)对图像显示有重要影响的索引数 如果是0 表示都很重要
}tBitMapInfoHeader;


/*BMP图片像素点结构体*/
typedef struct
{
	unsigned char blue;    //蓝色分量
	unsigned char green;   //绿色分量
	unsigned char red;     //红色分量
	unsigned char reserved; //保留值(亮度)
}tRGBquad;

typedef struct
{
	unsigned short width;   //位图宽度 单位像素
	unsigned short height; //位图高度 单位像素
	unsigned short bit_count; //位图像素格式 16bit 24bit 32bit
	const char *picture_path; //位图存放的路径
}tMapInof;

extern tMapInof bmp16_signet50x50_t;

extern tMapInof weather_code0_bmp51x51_t;
extern tMapInof weather_code1_bmp51x52_t;
extern tMapInof weather_code4_bmp60x47_t;
extern tMapInof weather_code5_bmp60x44_t;
extern tMapInof weather_code6_bmp60x51_t;
extern tMapInof weather_code7_bmp60x42_t;
extern tMapInof weather_code8_bmp56x49_t;
extern tMapInof weather_code9_bmp60x40_t;
extern tMapInof weather_code10_bmp60x59_t;
extern tMapInof weather_code11_bmp56x56_t;
extern tMapInof weather_code12_bmp56x56_t;
extern tMapInof weather_code13_bmp56x54_t;
extern tMapInof weather_code14_bmp56x54_t;
extern tMapInof weather_code15_bmp56x54_t;
extern tMapInof weather_code16_bmp56x54_t;
extern tMapInof weather_code17_bmp57x54_t;
extern tMapInof weather_code18_bmp57x54_t;
extern tMapInof weather_code99_bmp53x23_t;


int bmp16_bin_read(const char *bin_path, unsigned short length, unsigned char **buffer);
int bmp16_picture_read(const char *picture_path, unsigned char **buffer);


#endif /* __BMP_H */

/*---------------------------------The End------------------------------------*/
