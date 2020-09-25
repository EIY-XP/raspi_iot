/**
  -----------------------------------------------------------------------------
  * @file   : lcd_driver.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/03
  * @brief  : LCD驱动函数
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
	* @description
  *               LCD模块使用的控制器为ST7789VW,分辨率为240x320
	*               默认屏幕显示区为正向，即X=WIDTH=240, Y=HEIGHT=320
  *
  *	       B(0,0) |---------------|   ← X = 240 →
  * 	            |               |     
	*               |               |     ↑
	*               |               |     Y = 320
	*               |     GRAM      |     ↓
	*               |               |
  *               |               |
  *               |               |
  *               |               |
  *               |               |
  *               |---------------| E(240,320)
  *              扫描方式为：从左到右,从上到下
  * 
	* @history :
	* @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/

/* Includes ------------------------------------------------------------------*/
#include "lcd_driver.h"
#include "lcd_basic.h"
#include "../delay/delay.h"

#include <wiringPiSPI.h>
#include <wiringPi.h>

#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define SPI_CHAN 0
#define SPI_SPEED 10   //单位MHz 建议使用1M 10M 15.6M 31.2M (最高为32Mhz)
/* Private variables ---------------------------------------------------------*/
static int spi_fd;
static tLCDdevice lcd_dev_t;
/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/**
  * @function : lcd_lowlevel_init
  * @author   : xp
  * @brief    : LCD初始化 管脚 模式
  * @param    : 
  * @retval   : 
  */
void lcd_lowlevel_init(void)
{
  if ((spi_fd = wiringPiSPISetupMode(SPI_CHAN, (SPI_SPEED * 1000000), 0) < 0))
  {
		printf("open the spi bus fail !!!\n");
    return;
  }
  
  //使用wiringPi库初始化GPIO
  pinMode(LCD_RES_PIN, OUTPUT);
  pinMode(LCD_DC_PIN, OUTPUT);
  pinMode(LCD_CS_PIN, OUTPUT);
  pinMode(LCD_BK_PIN, OUTPUT);

  lcd_enable_chip(ENABLE);
  lcd_ctr_backlight(DISABLE);
  lcd_reset_chip();

  lcd_select_register(0x36);  //Memory Data Access Control(36H)
	lcd_write_data8bit(0x00);   //复位后默认为0

	lcd_select_register(0x3A);  //Interface Pixel Format
	lcd_write_data8bit(0x55);   //RGB5-6-5

	lcd_select_register(0xB2);  //Porch Setting 分5个步骤设置
	lcd_write_data8bit(0x0C);
	lcd_write_data8bit(0x0C);
	lcd_write_data8bit(0x00);
	lcd_write_data8bit(0x33);
	lcd_write_data8bit(0x33);

	lcd_select_register(0xB7); 
	lcd_write_data8bit(0x35);  

	lcd_select_register(0xBB);
	lcd_write_data8bit(0x19);

	lcd_select_register(0xC0);
	lcd_write_data8bit(0x2C);

	lcd_select_register(0xC2);
	lcd_write_data8bit(0x01);

	lcd_select_register(0xC3);
	lcd_write_data8bit(0x12);   

	lcd_select_register(0xC4);
	lcd_write_data8bit(0x20);  

	lcd_select_register(0xC6);  //Frame Rate Control in Normal Mode 帧频率
	lcd_write_data8bit(0x0F);   

	lcd_select_register(0xD0); 
	lcd_write_data8bit(0xA4);
	lcd_write_data8bit(0xA1);

	lcd_select_register(0xE0);
	lcd_write_data8bit(0xD0);
	lcd_write_data8bit(0x04);
	lcd_write_data8bit(0x0D);
	lcd_write_data8bit(0x11);
	lcd_write_data8bit(0x13);
	lcd_write_data8bit(0x2B);
	lcd_write_data8bit(0x3F);
	lcd_write_data8bit(0x54);
	lcd_write_data8bit(0x4C);
	lcd_write_data8bit(0x18);
	lcd_write_data8bit(0x0D);
	lcd_write_data8bit(0x0B);
	lcd_write_data8bit(0x1F);
	lcd_write_data8bit(0x23);

	lcd_select_register(0xE1);
	lcd_write_data8bit(0xD0);
	lcd_write_data8bit(0x04);
	lcd_write_data8bit(0x0C);
	lcd_write_data8bit(0x11);
	lcd_write_data8bit(0x13);
	lcd_write_data8bit(0x2C);
	lcd_write_data8bit(0x3F);
	lcd_write_data8bit(0x44);
	lcd_write_data8bit(0x51);
	lcd_write_data8bit(0x2F);
	lcd_write_data8bit(0x1F);
	lcd_write_data8bit(0x1F);
	lcd_write_data8bit(0x20);
	lcd_write_data8bit(0x23);

	lcd_select_register(0x21);  //Display Inversion On 显示反向控制
  lcd_select_register(0x11);  //Sleep Out 必须执行该步骤
	lcd_select_register(0x29); 	//Display Off

  lcd_set_parameters(direction0);
	// lcdBackLightCtr(ENABLE);
}


/**
  * @function : lcd_reset_chip
  * @author   : xp
  * @brief    : 复位
  * @param    : 
  * @retval   : 
  */
void lcd_reset_chip(void)
{
  digitalWrite(LCD_RES_PIN, HIGH);
  delay_msec(20);
  digitalWrite(LCD_RES_PIN, LOW);
  delay_msec(30);
  digitalWrite(LCD_RES_PIN, HIGH);
  delay_msec(20);
}


/**
  * @function : lcd_enable_chip
  * @author   : xp
  * @brief    : 片选使能/失能:0-使能；1-失能
  * @param    : 
  * @retval   : 
  */
void lcd_enable_chip(eFunctionState new_state)
{
  switch (new_state)
  {
    case ENABLE:   //低电平有效
      digitalWrite(LCD_CS_PIN, LOW);
      break;

    case DISABLE:
      digitalWrite(LCD_CS_PIN, HIGH);
      break;

    default:
      break;
  }
}


/**
  * @function : lcd_select_write_mode
  * @author   : xp
  * @brief    : 写入模式选择：0-寄存器 					 1-数据
  * @param    : 
  * @retval   : 
  */
void lcd_select_write_mode(eWriteMode mode)
{
  switch (mode)
  {
    case data_mode:
      digitalWrite(LCD_DC_PIN, HIGH);
      break;

    case command_mode:
      digitalWrite(LCD_DC_PIN, LOW);
      break;

    default:
      break;
  }
}


/**
  * @function : lcd_ctr_backlight
  * @author   : xp
  * @brief    : 背光控制
  * @param    : 
  * @retval   : 
  */
void lcd_ctr_backlight(eFunctionState new_state)
{
  switch (new_state)
  {
    case ENABLE:
      digitalWrite(LCD_BK_PIN, HIGH);
      break;

    case DISABLE:
      digitalWrite(LCD_BK_PIN, LOW);
      break;

    default:
      digitalWrite(LCD_BK_PIN, LOW);
      break;
  }
}


/**
  * @function : lcd_write_data8bit
  * @author   : xp
  * @brief    : 数据写入 8bit数据
  * @param    : 
  * @retval   : 
  */
void lcd_write_data8bit(uint8_t data)
{
  uint8_t byte;

  byte = data;
  lcd_enable_chip(ENABLE);
  lcd_select_write_mode(data_mode);
  wiringPiSPIDataRW(SPI_CHAN, &byte, 1);
  lcd_enable_chip(DISABLE);
}


/**
  * @function : lcd_write_data16bit
  * @author   : xp
  * @brief    : 数据写入 16bit数据
  * @param    : 
  * @retval   : 
  */
void lcd_write_data16bit(uint16_t data)
{
  uint8_t buffer[2];

  buffer[0] = data >> 8;
  buffer[1] = data & 0xff;
  lcd_enable_chip(ENABLE);
  lcd_select_write_mode(data_mode);
  wiringPiSPIDataRW(SPI_CHAN, buffer, 2);
  lcd_enable_chip(DISABLE);
}


/**
  * @function : lcd_select_register
  * @author   : xp
  * @brief    : 寄存器选择
  * @param    : 
  * @retval   : 
  */
void lcd_select_register(uint8_t reg)
{
  uint8_t byte;

  byte = reg;
  lcd_enable_chip(ENABLE);
  lcd_select_write_mode(command_mode);
  wiringPiSPIDataRW(SPI_CHAN, &byte, 1);
  lcd_enable_chip(DISABLE);
}


/**
  * @function : lcd_write_gram_ready
  * @author   : xp
  * @brief    : LCD的缓冲GRAM写入准备
  * @param    : 
  * @retval   : 
  */
void lcd_write_gram_ready(void)
{
  lcd_select_register(lcd_dev_t.write_gram_cmd);
}

/**
  * @function : lcd_set_parameters
  * @author   : xp
  * @brief    : 寄存器选择
  * @param    : 
  * @retval   : 
  */
void lcd_set_parameters(eLCDdirection dir)
{
  lcd_dev_t.id = LCD_320X240;
  lcd_dev_t.set_x_cmd = 0x2A;  //设置X坐标命令
  lcd_dev_t.set_y_cmd = 0x2B;  //设置Y坐命令
  lcd_dev_t.write_gram_cmd = 0x2C;  //写GRAM命令

  switch (dir)
  {
    case direction0:
      lcd_dev_t.width = SCREEN_WIDTH;
      lcd_dev_t.height = SCREEN_HEIGHT;
      lcd_select_register(0x36);
      lcd_write_data8bit(0); //BGR==1,MY==0,MX==0,MV==0
      break;

    case direction90:
      lcd_dev_t.width = SCREEN_HEIGHT;
      lcd_dev_t.height = SCREEN_WIDTH;
      lcd_select_register(0x36);
      lcd_write_data8bit((1 << 6) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
      break;

    case direction180:
      lcd_dev_t.width = SCREEN_WIDTH;
      lcd_dev_t.height = SCREEN_HEIGHT;
      lcd_select_register(0x36);
      lcd_write_data8bit((1 << 6) | (1 << 7)); //BGR==1,MY==0,MX==0,MV==0
      break;

    case direction270:
      lcd_dev_t.width = SCREEN_HEIGHT;
      lcd_dev_t.height = SCREEN_WIDTH;
      lcd_select_register(0x36);
      lcd_write_data8bit((1 << 7) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
      break;

    default:
      break;
  }
}


/**
  * @function : lcd_access_gram_region
  * @author   : xp
  * @brief    : 设置可访问的GRAM区域 设置完成后开启GRAM允许写入的命令
  * @param    : 
  * @retval   : 
  */
int lcd_access_gram_region(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
  /*
    [start_x] always must be equal to or less than [end_x]
    [start_y] always must be equal to or less than [end_y]
    0 < start_x [15:0] < end_x [15:0] < (lcd_dev_t.width - 1)
    0 < start_y [15:0] < end_y [15:0] < (lcd_dev_t.height - 1)
  */
  if ((end_x < start_x) || (end_y < start_y))
    return -1;

  if ((end_x > lcd_dev_t.width) || (end_y > lcd_dev_t.height))
    return -1;

  lcd_select_register(lcd_dev_t.set_x_cmd);
  lcd_write_data16bit(start_x);
  lcd_write_data16bit(end_x);

  lcd_select_register(lcd_dev_t.set_y_cmd);
  lcd_write_data16bit(start_y);
  lcd_write_data16bit(end_y);

  //开启GRAM允许写操作命令 之后可在已设置的区域写入数据
  lcd_write_gram_ready();

  return 0;
}


/**
  * @function : lcd_write_gram_region
  * @author   : xp
  * @brief    : 在GRAM中开辟一块区域 以color颜色填充
  * @param    : 
  * @retval   : 
  */
int lcd_write_gram_region(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t color)
{
  uint16_t x;
  uint16_t y;
  uint16_t x_total = (end_x - start_x);
  uint16_t y_total = (end_y - start_y);

  if (lcd_access_gram_region(start_x, start_y, end_x, end_y) < 0)
    return -1;
  
  for (y = 0; y < y_total; y++)
  {
    for (x = 0; x < x_total; x++)
    {
      lcd_write_data16bit(color);
    }
  }
  
  return 0;
}


/**
  * @function : lcd_set_cursor
  * @author   : xp
  * @brief    : 光标设置
  * @param    : 
  * @retval   : 
  */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
  lcd_access_gram_region(x, y, x, y);
}

/*---------------------------------The End------------------------------------*/

