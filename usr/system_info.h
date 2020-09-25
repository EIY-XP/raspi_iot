/**
  -----------------------------------------------------------------------------
  * @file   : system_info.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/13
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
  
#ifndef __SYSTEM_INFO_H
#define __SYSTEM_INFO_H



void get_iot_version(int *major, int *minor);
void get_system_calendar(char **date_p, char **time_p, unsigned char *week_day);
int get_cpu_temp(double *temp);


#endif /* __SYSTEM_INFO_H */

/*---------------------------------The End------------------------------------*/
