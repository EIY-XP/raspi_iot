/**
  -----------------------------------------------------------------------------
  * @file   : file.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/23
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2017 xxxx,All rights reserved  
  * @description
  *            写文件和日志
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/
/* Includes ------------------------------------------------------------------*/
#include "file.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>



/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER; //静态类型互斥量
const char *LOG_PATH = "/home/pi/workstation/eiy-project/log/raspi.log";


/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
int do_vasprintf(char **strp, const char *fmt, va_list ap);


/**
  * @function : write_file_test
  * @author   : xp
  * @brief    : 写文件测试 支持 参数的数量可变
  * @param    : 
  * @retval   : 
  */
int write_file_test(void)
{
	char *lib = "eiy_lib";
	char *name = "sxp";
	
	write_log_to_file("welcome to the %s, the author is %s", lib ,name);
	return 0;
}


/**
  * @function : write_data_to_file
  * @author   : xp
  * @brief    : 将数据写入指定的文件中 未添加互斥锁
  * @param    : 
  * @retval   : 
  */
int write_data_to_file(const char *file_path, char *str)
{
	FILE *fd = fopen(file_path, "a+");
	if (NULL == fd)
		return -1;

	if (str && (str[0] != 0))
	{
		fwrite(str, strlen(str), 1, fd);
		char *next = "\n";
		fwrite(next, strlen(next), 1, fd);
	}

	fclose(fd);
	return 0;
}



/**
  * @function : do_vasprintf
  * @author   : xp
  * @brief    : 执行vasprintf()格式化输出
  * @param    : 
  * @retval   : 
  */
int do_vasprintf(char **strp, const char *fmt, va_list ap)
{
	int ret;
 
	if (-1 == (ret = vasprintf(strp, fmt, ap)))
	{
		printf("Failed to vasprintf: %s. Bailing out\n", strerror(errno));
		return -1;
	}
	
	return ret;
}



/**
  * @function : write_log_to_file
  * @author   : xp
  * @brief    : 以互斥锁保护的方式写日志
  * @param    : 
  * @retval   : 
  */
int write_log_to_file(char *format, ...)
{
	int ret;
	FILE *fp = NULL;
	va_list vlist;
	char *fmt = NULL;

 	pthread_mutex_lock(&file_mutex);
 	
	if (0 == (fp = fopen(LOG_PATH, "a+"))) 
	{
		pthread_mutex_unlock(&file_mutex);
		return -1;
	}
 
	va_start(vlist, format);
	if (-1 == (ret = do_vasprintf(&fmt, format, vlist)))
	{
		va_end(vlist);
		fclose(fp);
		pthread_mutex_unlock(&file_mutex);
		return -1;
	}
	va_end(vlist);
	
	if (!fmt)
	{
		fclose(fp);
		pthread_mutex_unlock(&file_mutex);
		return -1;
	}
 
	time_t raw_time; //取系统时间
	struct tm *ptm = NULL;
	time(&raw_time);
	ptm = localtime(&raw_time);
	/*格式化写入数据*/
	fprintf(fp, "[%04d-%02d-%02d-%02d-%02d-%02d] %s\n",
					ptm->tm_year + 1900,
					ptm->tm_mon + 1,
					ptm->tm_mday,
					ptm->tm_hour,
					ptm->tm_min,
					ptm->tm_sec,
					fmt);
 
	free(fmt); //由于vasprintf()采用的是动态申请内存的方式 所以需要释放
	fsync(fileno(fp)); //写同步
	fclose(fp);
	pthread_mutex_unlock(&file_mutex);
	
	return 0;
} 


/*---------------------------------The End------------------------------------*/

