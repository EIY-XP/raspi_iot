/**
  -----------------------------------------------------------------------------
  * @file   : weather.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/20
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2017 xxxx,All rights reserved  
  * @description
  *            从“心知天气”网站获取天气信息
  *            需要注册免费账号，免费账号有以下权限
  *            (1)天气实况，包括天气现象文字、代码和气温 3 项数据
  *            (2)未来 3 天天气预报，包括白天天气现象文字及代码、晚间天气现象文字及代码、当天最高温度和最低温度、风向风速
  *            (3)6项基本类生活指数，包括穿衣、紫外线强度、洗车、旅游、感冒、运动指数
  *            
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/
/* Includes ------------------------------------------------------------------*/
#include "weather.h"
#include "cJSON.h"
#include "../delay/delay.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <errno.h>



/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* 心知天气（www.seniverse.com）IP及端口 */
#define  WEATHER_IP_ADDR   "116.62.81.138"
#define  WEATHER_PORT	   80

/* - 心知天气网注册秘钥 该密码有误 请自行注册 x - */
#define  KEY    "SxgSHnS8uOV7JBY7Q"		// 这是在心知天气注册后，每个用户自己的一个key

/* GET请求包 */
#define  GET_REQUEST_PACKAGE     \
         "GET https://api.seniverse.com/v3/weather/%s.json?key=%s&location=%s&language=zh-Hans&unit=c\r\n\r\n"
	
/* JSON数据包 还可以用更多其他的天气数据包可查阅心知天气*/	
#define  NOW_JSON     "now"

/* 提取后的天气数据 */	
tWeather weather_data = {0};
pthread_mutex_t weather_mutex = PTHREAD_MUTEX_INITIALIZER; //互斥锁

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

static int cJSON_nowWeatherParse(char *JSON, tWeather *result);
int get_weather(char *location, char *weather_json, tWeather *result);


/**
	* @function : get_weather_info_start
	* @author 	: xp
	* @brief		: 
	* @param		: 
	* @retval 	: 
	*/
int get_weather_info_start(pthread_t *tid)
{
	if (pthread_create(tid, NULL, (void*)pthread_get_weather, NULL))
    exit(-1);  //退出进程
  return 0;
}


/**
  * @function : pthread_get_weather
  * @author   : xp
  * @brief    : 获取天气信息的线程
  * @param    : 
  * @retval   : 
  */
void *pthread_get_weather(void *arg)
{
	int ret;
	char *location = "HangZhou";
	char *now_weather = "now";   //实时天气

	while (1)
	{
		memset(&weather_data, 0, sizeof(weather_data));
		
		pthread_mutex_lock(&weather_mutex);
		ret = get_weather(location, now_weather, &weather_data);
		pthread_mutex_unlock(&weather_mutex);
		
		if (-1 == ret)
			printf("get weather error\n");

		delay_sec(600);
	}

}


/**
  * @function : get_weather
  * @author   : xp
  * @brief    : 
  * @param    : *location 地区, *weather_json 获取天气类型, *result 返回数据结构体
  * @retval   : 
  */
int get_weather(char *location, char *weather_json, tWeather *result)
{
	int ret;
	int sock_fd;
	int sendbytes;
  char GetRequestBuf[256] = {0};
  char WeatherRecvBuf[1024] = {0};
    
  struct sockaddr_in serv_addr;
    
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_port=htons(WEATHER_PORT);
  serv_addr.sin_addr.s_addr=inet_addr(WEATHER_IP_ADDR);

  if(connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
  {
//		perror("connect fail!");
		return -1;
  }
  
  sprintf(GetRequestBuf, GET_REQUEST_PACKAGE, weather_json, KEY, location);
  if((sendbytes = send(sock_fd, GetRequestBuf, strlen(GetRequestBuf), 0)) == -1)
  {
//		perror("send fail");
		return -1;
  }
  
  ret = recv(sock_fd, WeatherRecvBuf, 1024, 0);  
	if (ret > 0)
	{
//		printf("Server return data is:\n %s\n",WeatherRecvBuf);    
  	/* 解析天气数据并保存到结构体变量weather_data中 */
  	if (0 == strcmp(weather_json, NOW_JSON))        // 天气实况
  	{
    	if (0 != (cJSON_nowWeatherParse(WeatherRecvBuf, result)))
    	{
				close(sock_fd);
    		return -1;
    	}	
 		}
	}
	else
	{
		close(sock_fd);
		return -1;
	}
 
  close(sock_fd);
  return 0;
}


/**
  * @function : cJSON_nowWeatherParse
  * @author   : xp
  * @brief    : 
  * @param    : 
  * @retval   : 
  */
static int cJSON_nowWeatherParse(char *JSON, tWeather *result)
{
	cJSON *json,*arrayItem,*object,*subobject,*item;
	
	json = cJSON_Parse(JSON); //解析JSON数据包
	if(json == NULL)			//检测JSON数据包是否存在语法上的错误，返回NULL表示数据包无效
	{
//		printf("Error before: [%s]\n",cJSON_GetErrorPtr()); //打印数据包语法错误的位置
		return -1;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL) //匹配字符串"results",获取数组内容
		{
			cJSON_GetArraySize(arrayItem); 		//获取数组中对象个数
			
			if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)//获取父对象内容
			{
				/* 匹配子对象1：城市地区相关 */
				if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
				{
					// 匹配id
					if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)	 
					{
						memcpy(result->id, item->valuestring,strlen(item->valuestring));		// 保存数据供外部调用
					}
						// 匹配城市名
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) 
					{
						memcpy(result->name, item->valuestring,strlen(item->valuestring));		// 保存数据供外部调用
					}
					// 匹配城市所在的国家
					if((item = cJSON_GetObjectItem(subobject,"country")) != NULL)
					{
						memcpy(result->country, item->valuestring,strlen(item->valuestring)); 	// 保存数据供外部调用
					}
					// 匹配完整地名路径
					if((item = cJSON_GetObjectItem(subobject,"path")) != NULL)	
					{
						memcpy(result->path, item->valuestring,strlen(item->valuestring));		// 保存数据供外部调用	
					}
					// 匹配时区
					if((item = cJSON_GetObjectItem(subobject,"timezone")) != NULL)
					{
						memcpy(result->timezone, item->valuestring,strlen(item->valuestring));	// 保存数据供外部调用	
					}
					// 匹配时差
					if((item = cJSON_GetObjectItem(subobject,"timezone_offset")) != NULL)
					{
						memcpy(result->timezone_offset, item->valuestring,strlen(item->valuestring)); 	// 保存数据供外部调用
					}
				}
				/* 匹配子对象2：今天的天气情况 */
				if((subobject = cJSON_GetObjectItem(object,"now")) != NULL)
				{
					// 匹配天气现象文字
					if((item = cJSON_GetObjectItem(subobject,"text")) != NULL)
					{
						memcpy(result->text, item->valuestring,strlen(item->valuestring));	// 保存数据供外部调用
					}
					// 匹配天气现象代码
					if((item = cJSON_GetObjectItem(subobject,"code")) != NULL)
					{
						memcpy(result->code, item->valuestring,strlen(item->valuestring));	// 保存数据供外部调用
					}
					// 匹配气温
					if((item = cJSON_GetObjectItem(subobject,"temperature")) != NULL) 
					{
						memcpy(result->temperature, item->valuestring,strlen(item->valuestring)); 	// 保存数据供外部调用
					} 
				}
				/* 匹配子对象3：数据更新时间（该城市的本地时间） */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					memcpy(result->last_update, subobject->valuestring,strlen(subobject->valuestring)); 	// 保存数据供外部调用
				}
			} 
		}
	}
		
	cJSON_Delete(json); //释放cJSON_Parse()分配出来的内存空间
		
	return 0;
}



/**
  * @function : display_weather
  * @author   : xp
  * @brief    : 
  * @param    : 
  * @retval   : 
  */
void display_weather_test(void)
{
	char *location = "HangZhou";

	memset(&weather_data, 0, sizeof(weather_data));   // weather_data清零 
	get_weather(location, NOW_JSON, &weather_data);   // GET 并解析实况天气数据
	
	printf("============%s today weather===========\n", weather_data.name);
	printf("weather_data->text: %s\n", weather_data.text);	
	printf("weather_code->code: %s\n",weather_data.code);
	printf("weather_data->temperature: %s\n", weather_data.temperature);	
	printf("weather_data->timezone: %s\n", weather_data.timezone);	
	printf("weather_data->timezone_offset: %s\n", weather_data.timezone_offset);
	printf("weather_data->last_update: %s\n", weather_data.last_update);
}
 
 
/*---------------------------------The End------------------------------------*/

