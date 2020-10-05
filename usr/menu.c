/**
  -----------------------------------------------------------------------------
  * @file   : menu.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/08/22
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
  * @description
  *           LCD菜单显示
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/

#include "includes.h"
#include "../file/file.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
char *week[7] = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
const char eth0_ip[IP_LENGTH] = "192.168.137.88";  //有线网卡IP地址
const char *LOG_FILE_PATH = "/home/pi/workstation/eiy-project/config/raspi_iot.txt";

/*天气图标结构体指针
	可以将 tMapInof *weather_bmp和char *weather合并在一个结构体中 比如叫weather_info结构体
 */
tMapInof *weather_bmp[20] = 
{
	&weather_code0_bmp51x51_t,
	&weather_code1_bmp51x52_t,
	&weather_code0_bmp51x51_t,
	&weather_code1_bmp51x52_t,
	&weather_code4_bmp60x47_t,
	&weather_code5_bmp60x44_t,
	&weather_code6_bmp60x51_t,
	&weather_code7_bmp60x42_t,
	&weather_code8_bmp56x49_t,
	&weather_code9_bmp60x40_t,
	&weather_code10_bmp60x59_t,
	&weather_code11_bmp56x56_t,
	&weather_code12_bmp56x56_t,
	&weather_code13_bmp56x54_t,
	&weather_code14_bmp56x54_t,
	&weather_code15_bmp56x54_t,
	&weather_code16_bmp56x54_t,
	&weather_code17_bmp57x54_t,
	&weather_code18_bmp57x54_t,  //特大暴雨图标
	&weather_code99_bmp53x23_t, //未知天气图标
};

//该数组内的 "天气" 与 weather_bmp数组内的"天气图标"必须一一对应
char *weather[20] = 
{
	"晴",
	"晴",
	"晴",
	"晴",
	"多云",
	"晴转多云",
	"晴转多云",
	"大部多云",
	"大部多云",
	"阴",
	"阵雨",
	"雷阵雨",
	"冰雹",
	"小雨",
	"中雨",
	"大雨",
	"暴雨",
	"大暴雨",
	"特大暴雨",
	"未知",
};


/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void display_menu(void);
void display_weather(void);
void display_network_info(void);
void display_cpu_temperature(void);
void display_system_calendar(void);
void display_modbus(void);


#if 0
/**
  * @function : lcd_display_chinese_test
  * @author   : xp
  * @brief    : 汉字显示测试
  * @param    : 
  * @retval   : 
  */
void lcd_display_chinese_test(void)
{
	char chinese_in[] = "树莓派";
	lcd_set_font(&hzk_font16x16_t);
  lcd_display_chinese(10,220,chinese_in);
}


void lcd_display_ascii_test(void)
{
	unsigned char ascii_in[] = "raspberryPi 4b";
	lcd_set_font(&asc_font8x16_t);
	lcd_display_srting(10,250,ascii_in);
}

#endif


/**
	* @function : display_device_info_start
	* @author 	: xp
	* @brief		: 
	* @param		: 
	* @retval 	: 
	*/
int display_device_info_start(pthread_t *tid)
{
	lcd_init();
	if (pthread_create(tid, NULL, (void*)thread_display_device_info, NULL))
    exit(-1);  //退出进程
  return 0;
}


/**
  * @function : display_menu
  * @author   : xp
  * @brief    : LCD菜单
  * @param    : 
  * @retval   : 
  */
void display_menu(void)
{
	lcd_set_text_color(COLOR_BLUE2);
	/*raspberryPi logo*/
	lcd_display_icon(2, 2, &bmp50x60_raspberry_t);
	/*上中横线*/
	lcd_draw_line(0, 112, 240, 0);
	lcd_draw_line(0, 113, 240, 0);
	/*下中横线*/
	lcd_draw_line(0, 182, 240, 0);
	lcd_draw_line(0, 183, 240, 0);

	/*右中竖线*/
	lcd_draw_line(218, 113, 69, 1);
	lcd_draw_line(219, 113, 69, 1);

	lcd_set_font(&asc_font8x16_t);
	lcd_display_srting(55, 5, (unsigned char*)"eth0 :");     //eth0
	lcd_display_srting(104, 5, (unsigned char*)eth0_ip);       //eth0固定IP
	lcd_display_srting(55, 23, (unsigned char*)"wlan0:");      //wlan0
	lcd_display_srting(55, 41, (unsigned char*)"cpu temp:"); //cpu temp
	lcd_display_temp_icon(145, 41, bmp16x16_tem_table);        //℃

	lcd_set_font(&hzk_font16x16_t);
	lcd_display_bmp(2, 116, weather_bmp[19]);   //未知天气图标
	lcd_display_chinese(113, 131, weather[19]); //未知天气
	
	lcd_display_chinese(220, 114, (char*)"天"); //天气信息栏
	lcd_display_chinese(220, 131, (char*)"气");
	lcd_display_chinese(220, 148, (char*)"信");
	lcd_display_chinese(220, 165, (char*)"息");

	lcd_display_chinese(65, 114, (char*)"城市");     //城市
	lcd_display_chinese(65, 131, (char*)"天气");     //天气
	lcd_display_chinese(65, 148, (char*)"气温");     //气温
	lcd_display_chinese(65, 165, (char*)"空气质量");
	lcd_ctr_backlight(ENABLE);
	lcd_display_temp_icon(130, 148, bmp16x16_tem_table); //℃

	lcd_display_bmp(190, 270, &bmp16_signet50x50_t);

}


/**
  * @function : display_weather
  * @author   : xp
  * @brief    : 显示天气信息
  * @param    : 
  * @retval   : 
  */
void display_weather(void)
{
	int flag = 0, i = 0;
	unsigned short int color;
	unsigned short int weather_code = 19;

	for (i = 0; i < 2; i++)
	{
		/* 为了防止 获取天气信息 的线程挂掉导致互斥量无法释放 最多尝试2次获取 */
		if (0 != (flag = pthread_mutex_trylock(&weather_mutex)))
		{	
			delay_sec(2);
			continue;
		}
			
		weather_code = atoi(weather_data.code);
		if ((weather_code < 0) || (weather_code > 19))  //防止数组下标越界
			weather_code = 19;
		lcd_get_back_color(&color);   //获取背景颜色
		lcd_set_font(&hzk_font16x16_t);
		lcd_write_gram_region(2, 116, 62, 176,color);  //清空显示区
		lcd_display_bmp(2, 116, weather_bmp[weather_code]);    //刷新天气图标信息
		lcd_write_gram_region(113, 114, 177, 130, color);  //清空4个汉字显示区
		lcd_display_chinese(113, 114, (char*)weather_data.name); //刷新城市名称
		lcd_write_gram_region(113, 131, 177, 147, color);  //清空4个汉字显示区
	//		lcd_display_chinese(113, 131, (char*)weather_data.text);
		lcd_display_chinese(113, 131, weather[weather_code]);   //刷新天气描述

		lcd_set_font(&asc_font8x16_t);
		lcd_write_gram_region(113, 148, 129, 164, color);      //清空1个汉字显示区
		lcd_display_srting(113, 152, (unsigned char*)weather_data.temperature); //刷新气温
		lcd_display_srting(145, 165, (unsigned char*)"N/A"); //刷新空气质量
		pthread_mutex_unlock(&weather_mutex);
		
		break; //如果第一次就能顺利获取 则跳出
	}
}


/**
  * @function : display_network_info
  * @author   : xp
  * @brief    : 网卡信息
  * @param    : 
  * @retval   : 
  */
void display_network_info(void)
{
	unsigned short int color;

	const char *raspi_wlan0 = "wlan0";  //无线网卡名称
	char ip_addr[IP_LENGTH] = {0};       //无线网卡IP地址

	lcd_get_back_color(&color);
	lcd_set_font(&asc_font8x16_t);

	if (0 == get_local_ipv4_address(raspi_wlan0, ip_addr))
	{
		lcd_write_gram_region(104, 23, 224, 39, color); 
		lcd_display_srting(104, 23, (unsigned char*)ip_addr);
	}
	else
	{
		lcd_write_gram_region(104, 23, 224, 39, color); 
		lcd_display_srting(104, 23, (unsigned char*)"not connect");
	}
}



/**
  * @function : display_cpu_temperature
  * @author   : xp
  * @brief    : CPU当前温度
  * @param    : 
  * @retval   : 
  */
void display_cpu_temperature(void)
{
  char buf[10];
  double cpu_temp = 0;     //CPU温度值变量

  if (0 == get_cpu_temp(&cpu_temp))
  {
		sprintf(buf, "%d", (unsigned short int)cpu_temp); //1位精度输出
		
		lcd_set_font(&asc_font8x16_t);
		lcd_display_srting(128, 41, (unsigned char*)buf);
  }
}


/**
  * @function : display_system_calendar
  * @author   : xp
  * @brief    : 系统时间 日历
  * @param    : 
  * @retval   : 
  */
void display_system_calendar(void)
{
	char *chinese_in;
	unsigned char week_day = 0;  //星期
	char *date_str;	//日期
	char *time_str;	//系统时间

	get_system_calendar(&date_str, &time_str, &week_day);
	
	lcd_set_font(&asc_font8x16_t);
	lcd_display_srting(140, 70, (unsigned char*)date_str);
	lcd_set_font(&asc_font24x48_t);
	lcd_display_srting(8, 62, (unsigned char*)time_str);
	chinese_in = week[week_day];

	lcd_set_font(&hzk_font16x16_t);
  lcd_display_chinese(140, 88, chinese_in);
}


void display_modbus(void)
{
  char temp_buf[10];
  char hum_buf[10];
  
	sem_wait(&sem_sensor);
	sprintf(temp_buf, "%.1f", sensor_temp); //1位精度输出
	sprintf(hum_buf, "%.1f", sensor_hum);    //1位精度输出
	sem_post(&sem_sensor);

  lcd_set_font(&asc_font8x16_t);
  lcd_display_srting(10, 280, (unsigned char*)"Temp=");
  lcd_display_srting(52, 280, (unsigned char*)temp_buf);
  lcd_display_srting(10, 300, (unsigned char*)"hum =");
  lcd_display_srting(52, 300, (unsigned char*)hum_buf);
}


/**
  * @function : thread_display_device_info
  * @author   : xp
  * @brief    : 线程 设备信息显示-网络IP 系统时间 CPU温度值 电量显示
  * @param    : 
  * @retval   : 
  */
void *thread_display_device_info(void *arg)
{
	unsigned short int   i = 0, j = 0;

	display_menu();
	delay_sec(15);
	display_cpu_temperature();
	display_system_calendar();
	display_network_info();
	display_weather();
	
	write_log_to_file((char*)"the raspi_iot app start");
	
	while (1)
	{
		display_system_calendar();
		display_cpu_temperature();
		display_modbus();

		/* 每10分钟重新获取一次天气信息 */	
		if (i++ == 30) 
		{
			display_weather();
			i = 0;
		}

		/* 每30分钟重新获取一次IP */
		if (j++ == 180)  
		{
//			display_network_info();
//			write_log_to_file((char*)"the raspi_iot app is running");
			j = 0;
		}

		delay_sec(10);
	}
	
	return NULL;
}


/*---------------------------------The End------------------------------------*/


