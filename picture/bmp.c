/**
  -----------------------------------------------------------------------------
  * @file   : bmp.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/28
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2017 xxxx,All rights reserved  
  * @description
  *            读取BMP图片 BMP图片存储使用小端模式(低地址存放地位数据, 高地址存放高位数据)
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/
/* Includes ------------------------------------------------------------------*/
#include "bmp.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
const char *BMP_PATH = "/home/pi/workstation/eiy-project/config/qq_logo16.bmp";

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

tMapInof bmp16_qq40x40_t = 
{
	40,
	40,
	16,
	"/home/pi/workstation/eiy-project/config/qq_logo16.bin",
};


tMapInof bmp16_signet50x50_t = 
{
	50,
	50,
	16,
	"/home/pi/workstation/eiy-project/config/signet.bin",
};

/*
	weather code0和code2 晴天 白天
*/
tMapInof weather_code0_bmp51x51_t = 
{
	51,
	51,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code0_51x51.bin",
};

/*
	weather code1和code3 晴天 夜晚
*/
tMapInof weather_code1_bmp51x52_t = 
{
	51,
	52,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code1_51x52.bin",
};

/*
	weather code4 多云
*/
tMapInof weather_code4_bmp60x47_t = 
{
	60,
	47,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code4_60x47.bin",
};

/*
	weather code5 晴间多云 白天
*/
tMapInof weather_code5_bmp60x44_t = 
{
	60,
	44,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code5_60x44.bin",
};

/*
	weather code6 晴间多云 夜晚
*/
tMapInof weather_code6_bmp60x51_t = 
{
	60,
	51,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code6_60x51.bin",
};


/*
	weather code7 大部多云 白天
*/
tMapInof weather_code7_bmp60x42_t = 
{
	60,
	42,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code7_60x42.bin",
};

/*
	weather code8 大部多云 夜晚
*/
tMapInof weather_code8_bmp56x49_t = 
{
	56,
	49,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code8_56x49.bin",
};

/*
	weather code9 阴
*/
tMapInof weather_code9_bmp60x40_t = 
{
	60,
	40,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code9_60x40.bin",
};

/*
	weather code10 阵雨
*/
tMapInof weather_code10_bmp60x59_t = 
{
	60,
	59,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code10_60x59.bin",
};

/*
	weather code11 雷阵雨
*/
tMapInof weather_code11_bmp56x56_t = 
{
	56,
	56,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code11_56x56.bin",
};

/*
	weather code12 雷阵雨伴有冰雹
*/
tMapInof weather_code12_bmp56x56_t = 
{
	56,
	56,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code12_56x56.bin",
};

/*
	weather code13 小雨
*/
tMapInof weather_code13_bmp56x54_t = 
{
	56,
	54,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code13_56x54.bin",
};

/*
	weather code14 中雨
*/
tMapInof weather_code14_bmp56x54_t = 
{
	56,
	54,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code14_56x54.bin",
};

/*
	weather code15 大雨
*/
tMapInof weather_code15_bmp56x54_t = 
{
	56,
	54,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code15_56x54.bin",
};


/*
	weather code16 暴雨
*/
tMapInof weather_code16_bmp56x54_t = 
{
	56,
	54,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code16_56x54.bin",
};

/*
	weather code17 大暴雨
*/
tMapInof weather_code17_bmp56x54_t = 
{
	56,
	54,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code17_57x54.bin",
};

/*
	weather code18 大暴雨
*/
tMapInof weather_code18_bmp57x54_t = 
{
	57,
	54,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code18_57x54.bin",
};

/*
	weather code99 未知
*/
tMapInof weather_code99_bmp53x23_t = 
{
	53,
	23,
	16,
	"/home/pi/workstation/eiy-project/config/weather_config/weather_code99_53x23.bin",
};



/**
  * @function : bmp16_bin_read
  * @author   : xp
  * @brief    : 读取bmp图片二进制文件 即去掉了图片的头部信息 只保留数据部分
  * @param    : 
  * @retval   : 
  */
int bmp16_bin_read(const char *bin_path, unsigned short length, unsigned char **buffer)
{
	int ret = 0;
	FILE *fd = fopen(bin_path, "r");
	if (NULL == fd)
		return -1;

	fseek(fd, 0, SEEK_SET);
	ret = fread(*buffer, length, 1, fd);
	if (ret <= 0)
	{
		fclose(fd);
		return -1;
	}

	fclose(fd);
	return 0;
}




/**
  * @function : bmp_picture_read
  * @author   : xp
  * @brief    : 直接读取bmp文件(.bmp格式)
  * @param    : 
  * @retval   : 
  */
int bmp16_picture_read(const char *picture_path, unsigned char **buffer)
{
	tBitMapFileHeader fileHeader;
	tBitMapInfoHeader infoHeader;
	unsigned char buffer_test[54];
	unsigned short i, j;

	FILE *fd = fopen(picture_path, "r");
	if (NULL == fd)
		return -1;
		
	fseek(fd, 0, SEEK_SET); 
	fread(buffer_test, 54, 1, fd); 
	for (i = 0; i < 54; i++)
	{
		printf("0x%x ", buffer_test[i]);
	}	

	printf("\n");
	
	fseek(fd, 0, SEEK_SET); 
	fread(&fileHeader.bfType, sizeof(fileHeader.bfType), 1, fd); 
	fseek(fd, 10, SEEK_SET); 
	fread(&fileHeader.bfOffBit, sizeof(fileHeader.bfOffBit), 1, fd);
	fseek(fd, 28, SEEK_SET); 
	fread(&infoHeader.biBitCount, sizeof(infoHeader.biBitCount), 1, fd);
	fseek(fd, 34, SEEK_SET); 
	fread(&infoHeader.biSizeImage, sizeof(infoHeader.biSizeImage), 1, fd);

	if ((0 != memcmp(fileHeader.bfType, "BM", 2)) && (54 != fileHeader.bfOffBit) 
	    && (16 != infoHeader.biBitCount))
	{
		fclose(fd);
		return -1;
	}

	printf("fileHeader.bfType = %s\n", fileHeader.bfType);
	printf("fileHeader.bfOffBit = %ld\n", fileHeader.bfOffBit);
	printf("infoHeader.biBitCount = %d\n", infoHeader.biBitCount);
	printf("infoHeader.biSizeImage = %ld\n", infoHeader.biSizeImage);

	fseek(fd, 54, SEEK_SET); 
	fread(*buffer, infoHeader.biSizeImage, 1, fd);
	for (i = 0; i < 200; i++)
	{
		for (j = 0; j < 16; j++)
		{
			printf("0x%x ", *(*buffer + i*16 + j));
		}
		printf("\n");
	}
	

	fclose(fd);
	return infoHeader.biSizeImage;
}
 
 
/*---------------------------------The End------------------------------------*/
