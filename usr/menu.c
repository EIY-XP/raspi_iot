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
/* 项目自带头文件 */
#include "includes.h"
#include "../file/file.h"
#include "../lcd/lcd_basic.h"
#include "../lcd/lcd_driver.h"
#include "../lcd/lcd_fonts.h"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>



/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
sem_t sem_sensor;

double cpu_temp = 0;     //CPU温度值变量

char date_str[20] = {0};	//日期
char time_str[10] = {0};	//系统时间
unsigned char week_day;  //星期
char *week[7] = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};

char eth0_ip[IP_LENGTH] = {0};  //有线网卡IP地址
char wlan0_ip[IP_LENGTH] = {0}; //无线网卡IP地址
char mac[MAC_LENGTH] = {0};      //MAC地址

tBmp *weather_bmp[20] = 
{
	&weather_code99_bmp53x23_t, //未知天气图标
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
	&weather_code17_bmp56x54_t,
	&weather_code18_bmp57x54_t  //特大暴雨图标
};

//该数组内的 "天气" 与 weather_bmp数组内的"天气图标"一一对应
char *weather[20] = 
{
	"未知",
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
	"特大暴雨"
};

const char *LOGO_FILE_PATH = "/home/pi/workstation/eiy-project/log/pi-iot.txt";


/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void display_menu(void);
void display_weather(char *location, char *weather_json);
void display_network_info(void);
void display_cpu_temperature(void);
void display_system_calendar(void);
void display_modbus(void);




	
/**
	* @function : display_device_info_start
	* @author 	: xp
	* @brief		: 
	* @param		: 
	* @retval 	: 
	*/
int display_device_info_start(void)
{
	pthread_t tid_lcd;

	if (pthread_create(&tid_tcp_server, NULL, (void*)thread_tcp_server, NULL))
    exit(-1);  //退出进程

	pthread_detach(tid_lcd);	
}


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


/**
  * @function : display_menu
  * @author   : xp
  * @brief    : 
  * @param    : 
  * @retval   : 
  */
void display_menu(void)
{
	lcd_set_text_color(COLOR_BLUE);
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
	lcd_display_srting(55, 23, (unsigned char*)"wlan0:");      //wlan0
	lcd_display_srting(55, 41, (unsigned char*)"cpu temp:"); //cpu temp
	lcd_display_temp_icon(145, 41, bmp16x16_tem_table);        //℃

	lcd_display_icon(2, 116, weather_bmp[0]);   //未知天气图标
	lcd_set_font(&hzk_font16x16_t);
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
}


/**
  * @function : display_weather
  * @author   : xp
  * @brief    : 
  * @param    : 
  * @retval   : 
  */
void display_weather(char *location, char *weather_json)
{
	unsigned short int color;
	tWeather weather_data = {0};
	unsigned short int weather_code = 0;

	memset(&weather_data, 0, sizeof(weather_data));   // weather_data清零 
	
	if (0 == (get_weather(weather_json, location, &weather_data)))  // GET 并解析实况天气数据
	{
		weather_code = atoi(weather_data.code);
		
		lcd_get_back_color(&color);   //获取背景颜色
		lcd_set_font(&hzk_font16x16_t);
		lcd_write_gram_region(2, 116, 62, 176,color);  //清空显示区
		lcd_display_icon(2, 116, weather_bmp[weather_code+1]); //刷新天气图标信息
		lcd_write_gram_region(113, 114, 177, 130, color);  //清空4个汉字显示区
		lcd_display_chinese(113, 114, (char*)weather_data.name); //刷新城市名称
		lcd_write_gram_region(113, 131, 177, 147, color);  //清空4个汉字显示区
//		lcd_display_chinese(113, 131, (char*)weather_data.text);
		lcd_display_chinese(113, 131, weather[weather_code+1]); //刷新天气描述

		lcd_set_font(&asc_font8x16_t);
		lcd_write_gram_region(113, 148, 129, 164, color);  //清空1个汉字显示区
		lcd_display_srting(113, 152, (unsigned char*)weather_data.temperature); //刷新气温
		lcd_display_srting(145, 165, (unsigned char*)"N/A"); //刷新空气质量
	}
}


/**
  * @function : display_network_info
  * @author   : xp
  * @brief    : 网络信息
  * @param    : 
  * @retval   : 
  */
void display_network_info(void)
{
	unsigned short int color;

	const char *raspi_eth0 = "eth0";    //有线网卡名称
	const char *raspi_wlan0 = "wlan0";  //无线网卡名称

	lcd_get_back_color(&color);
	lcd_set_font(&asc_font8x16_t);
	
	if (0 == get_local_ipv4_address(raspi_eth0, eth0_ip))
	{
		lcd_write_gram_region(104, 5, 224, 22, color); 
		lcd_display_srting(104, 5, (unsigned char*)eth0_ip);
		//	printf("local %s ip: %s\n", test_eth, eth0_ip);
	}
	else
	{
		lcd_write_gram_region(104, 5, 224, 22, color); 
		lcd_display_srting(104, 5, (unsigned char*)"not connect");
	}

	if (0 == get_local_ipv4_address(raspi_wlan0, wlan0_ip))
	{
		lcd_write_gram_region(104, 23, 224, 39, color); 
		lcd_display_srting(104, 23, (unsigned char*)wlan0_ip);
		//	printf("local %s ip: %s\n", test_wlan, wlan0_ip);
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
  * @brief    : LCD显示CPU当前温度
  * @param    : 
  * @retval   : 
  */
void display_cpu_temperature(void)
{
  char buf[10];

  if (0 == get_cpu_temp())
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

	get_system_calendar();
	
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
	unsigned int   i;
	char *location = "HangZhou";
	char *now_weather = "now";   //实时天气

	display_menu();
	
	delay_sec(10);
	display_cpu_temperature();
	display_system_calendar();
	display_network_info();
	display_weather(location, now_weather);
	write_log_to_file((char*)"the pi-iot app start");
	
	while (1)
	{
		++i;
		
		if (0 == (i % 20)) //每20秒钟重新获取一次时间和CPU温度
		{
			display_system_calendar();
			display_cpu_temperature();
			display_modbus();
		}
			
		if (0 == (i % 600)) //每10分钟重新获取一次天气信息
		{
			display_weather(location, now_weather);
		}

		if(0 == (i % 1800))  //每30分钟重新获取一次IP
		{
			display_network_info();
			write_log_to_file((char*)"the pi-iot app running ok");
		}

		delay_sec(1);
	}
	
	return NULL;
}


/*---------------------------------The End------------------------------------*/


