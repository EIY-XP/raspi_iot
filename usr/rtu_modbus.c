/**
  -----------------------------------------------------------------------------
  * @file   : rtu_modbus.c
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/12
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
  * @description
  *          
  *            RTU MODBUS通信 
  *						 使用UART2("/dev/ttyAMA1")
  *		         管脚：GPIO0    = TXD2 (排针的27引脚)
  *                  GPIO1 = RXD2 (排针的28引脚)
  *            raspberry 4b默认情况下UART2是失能的，需要在/boot/config.txt文件中配置
  						 配置为：dtoverlay=uart2
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/

/* Includes ------------------------------------------------------------------*/
#include "rtu_modbus.h"
#include "../delay/delay.h"
#include "../common/common_lib.h"
#include "variables.h"

#include <wiringSerial.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
const char *uart_port = "/dev/ttyAMA1";
char mb_frames_buf[100] = {0};
static char send_status = 0;
static char rev_status = 0;

/* Public variables ----------------------------------------------------------*/
sem_t sem_sensor;
double sensor_temp = 0.0; 
double sensor_hum = 0.0;

/* Private function prototypes -----------------------------------------------*/

/* Exported function declare -------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @function : modbus_poll_start
  * @author   : xp
  * @brief    : 
  * @param    : 
  * @retval   : 
  */
int modbus_poll_start(void)
{
	pthread_t tid_modbus;
	void *thread_modbus_rev = NULL;
	
	sem_init(&sem_sensor, 0, 1); //信号量初始化
	if (pthread_create(&tid_modbus, NULL, (void*)thread_modbus_poll,  com_port))
	   exit(-1);

	pthread_join(tid_modbus, &thread_modbus_rev);
	if (*(int*)thread_modbus_rev == -1)  //线程异常退出
		return -1;
}



/**
  * @function : rtu_bus_status
  * @author   : xp
  * @brief    : 基于“问答”式 串口总线状态(只能是 <发送> 或者 <接受>)
  * @param    : 
  * @retval   : 
  */
void rtu_bus_status(char new_status)
{
	switch (new_status)
	{
		case rx_status:
			send_status = 0;
			rev_status = 1;
			break;

		case tx_status:
			send_status = 1;
			rev_status = 0;
			break;

		default:
			send_status = 0;
			rev_status = 0;
			break;
	}
}


/**
  * @function : func03H_send_message
  * @author   : xp
  * @brief    : 03H功能码数据帧组成
  * @param    : 
  * @retval   : 返回 数据帧长度
  */
int func03H_send_message(char* frames, unsigned short int slave_addr, unsigned short int read_addr, unsigned short int read_num)
{
  unsigned short int   index = 0;
  unsigned short int crc16   = 0;
  
  frames[index++] = slave_addr;
  frames[index++] = MB_FUNC_READ_HOLDING_REGISTER;
  frames[index++] = read_addr >> 8;
  frames[index++] = read_addr;
  frames[index++] = read_num >> 8;
  frames[index++] = read_num;
  
  /*计算CRC的值*/
  crc16 = crc16_calculate(frames, index);
  
  /*低字节发送在前*/
  frames[index++] = (crc16 & 0x0FF);
  frames[index++] = (crc16 >> 8);

  return index;
}





/**
  * @function : thread_modbus_poll
  * @author   : xp
  * @brief    : 
  * @param    : 
  * @retval   : 
  */
void *thread_modbus_poll(void *arg)
{

	int fd;
	unsigned short int addr;
	unsigned short int num_reg;
	unsigned short int crc16;
	unsigned short int length;

	if ((fd = serialOpen(arg, 115200)) < 0)
	{
		pthread_exit((void*)-1);
	}
	
	crc16 = 0;
	length = 0;
	addr = 1;
	num_reg = 2;
	memset(mb_frames_buf, 0, sizeof(mb_frames_buf));
	rtu_bus_status(tx_status);  //失能发送
	
	for (; ;)
	{
		if ((send_status == 1) && (rev_status == 0))
		{
			length =  func03H_send_message(mb_frames_buf, 1, (addr - 1), num_reg);
			write(fd, mb_frames_buf, length);
			rtu_bus_status(rx_status);
			memset(mb_frames_buf, 0, sizeof(mb_frames_buf));
			delay_msec(300);
		}
		else if ((send_status == 0) && (rev_status == 1))
		{
			length = read(fd, mb_frames_buf, sizeof(mb_frames_buf));
			if (((num_reg * 2 + 5) == length) && (mb_frames_buf[1] == MB_FUNC_READ_HOLDING_REGISTER))
			{
				crc16 = (mb_frames_buf[length -1] << 8) | (mb_frames_buf[length -2]);
				if (crc16 == crc16_calculate(mb_frames_buf, length-2))
				{					
					sem_wait(&sem_sensor);
					sensor_temp = (float)(mb_frames_buf[3] << 8 | mb_frames_buf[4]) / 10;
					sensor_hum = (float)(mb_frames_buf[5] << 8 | mb_frames_buf[6]) / 10;
					sem_post(&sem_sensor);
				}
			}

			memset(mb_frames_buf, 0, sizeof(mb_frames_buf));
			rtu_bus_status(tx_status);
			delay_msec(20);
		}
	}

	pthread_exit(NULL);
}


/*---------------------------------The End------------------------------------*/


