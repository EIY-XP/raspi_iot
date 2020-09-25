/**
  -----------------------------------------------------------------------------
  * @file   : system_info.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/13
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2017 xxxx,All rights reserved  
  * @description
  *            
  *            获取系统信息 包括时间 无线网络状态以及获取的IP CPU温度等
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "version.h"

/* C/C++标准库头文件 */
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
/* 提供open()函数 */ 
#include <fcntl.h>          
#include <sys/types.h>      
#include <sys/stat.h>  
/* 提供close()函数 */
#include <unistd.h>   


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
const char *TEMP_PATH = "/sys/class/thermal/thermal_zone0/temp";  //CPU温度查询文件
char date_str[20] = {0};	//日期
char time_str[10] = {0};	//系统时间

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/**
  * @function : get_iot_version
  * @author   : xp
  * @brief    : 获取软件版本信息
  * @param    : 
  * @retval   : 
  */
void get_iot_version(int *major, int *minor)
{
  *major = MAJOR_VERSION;
  *minor = MINOR_VERSION;
}


/**
  * @function : get_system_calendar
  * @author   : xp
  * @brief    : 获取系统当前时间
  * @param    : 
  * @retval   : 
  */
void get_system_calendar(char **date_p, char **time_p, unsigned char *week_day)
{
  /*
  struct tm {
  int tm_sec;         // 秒，范围从 0 到 59
  int tm_min;         // 分，范围从 0 到 59 
  int tm_hour;        // 小时，范围从 0 到 23 
  int tm_mday;        // 一月中的第几天，范围从 1 到 31
  int tm_mon;         // 月份，范围从 0 到 11 
  int tm_year;        // 自 1900 起的年数  (当前年份=tm_year+1900)
  int tm_wday;        // 一周中的第几天，范围从 0 到 6  (0:表示星期天)
  int tm_yday;        // 一年中的第几天，范围从 0 到 365
  int tm_isdst;       // 夏令时   
  };
  包含了可读格式的日期和时间信息 localtime（）- Www Mmm dd hh:mm:ss yyyy，
  其中，Www 表示星期几，Mmm 是以字母表示的月份，dd 表示一月中的第几天，hh:mm:ss 表示时间，yyyy 表示年份

  strftime（）函数：识别以百分号(%)开始的格式命令集合，格式如下说明
  格式命令，是区分大小写的：
  %a 星期几的简写
  %A 星期几的全称
  %b 月分的简写
  %B 月份的全称
  %c 标准的日期的时间串
  %C 年份的后两位数字
  %d 十进制表示的每月的第几天
  %D 月/天/年
  %e 在两字符域中，十进制表示的每月的第几天
  %F 年-月-日
  %g 年份的后两位数字，使用基于周的年
  %G 年分，使用基于周的年
  %h 简写的月份名
  %H 24小时制的小时
  %I 12小时制的小时
  %j 十进制表示的每年的第几天
  %m 十进制表示的月份
  %M 十时制表示的分钟数
  %n 新行符
  %p 本地的AM或PM的等价显示
  %r 12小时的时间
  %R 显示小时和分钟：hh:mm
  %S 十进制的秒数
  %t 水平制表符
  %T 显示时分秒：hh:mm:ss
  %u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
  %U 第年的第几周，把星期日做为第一天（值从0到53）
  %V 每年的第几周，使用基于周的年
  %w 十进制表示的星期几（值从0到6，星期天为0）
  %W 每年的第几周，把星期一做为第一天（值从0到53）
  %x 标准的日期串
  %X 标准的时间串
  %y 不带世纪的十进制年份（值从0到99）
  %Y 带世纪部分的十进制年份
  %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
  %% 百分号
  */

	time_t raw_time;  //原始时间戳
  struct tm* time_info;

  time(&raw_time);  //获取当前系统时间的原始时间戳
  // time_info = gmtime(&raw_time);  //将时间戳按照tm数据结构提取
  time_info = localtime(&raw_time);  //将时间戳按照tm数据结构提取

	strftime(date_str, sizeof(date_str), "%F", time_info);
	strftime(time_str, sizeof(time_str), "%R", time_info);
	
	*week_day = (unsigned char)time_info->tm_wday;
	*date_p = date_str;
	*time_p = time_str;
	
  #if 0
  printf("China:%2d:%02d\n", time_info->tm_hour+CCT, time_info->tm_min);
  printf("China:%4d/%02d/%02d/%d\n", time_info->tm_year+1900, time_info->tm_mon+1, time_info->tm_mday, time_info->tm_wday);
  #endif
}


/**
  * @function : get_cpu_temp
  * @author   : xp
  * @brief    : 获取CPU温度
  * @param    : 
  * @retval   : 
  */
int get_cpu_temp(double *temp)
{
	int fd;  //设备描述符
	char buf[10];
	
	#if 0
	pid_t pid = getpid();
	pthread_t tid = pthread_self();
	printf("%s pid: %u, tid: %u (0x%x)\n", (char*)arg, (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
	#endif
	
	//以只读方式打开/sys/class/thermal/thermal_zone0/temp 
 	if ((fd = open(TEMP_PATH, O_RDONLY)) < 0)
		return -1;

 	if (read(fd, buf, 10) < 0)
 	{
		close(fd);
		return -1;
 	}


  /* 
   转换为浮点数打印 
   使用atoi函数将所指向的字符串转换为一个整数(类型为 int 型) 再除1000保留小数点一位
  */
  *temp = atoi(buf) / 1000.0;
//  printf("temp:%.1f\n", cpu_temp);

	return 0;
}




/*---------------------------------The End------------------------------------*/
