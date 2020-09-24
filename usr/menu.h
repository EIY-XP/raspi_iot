/**
  -----------------------------------------------------------------------------
  * @file   : menu.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/08/22
  * @brief  : 
  -----------------------------------------------------------------------------
  *              Copyright (c) 2020 xxxx,All rights reserved  
  * @description
  *              
  * @history :
  * @date    :
  * @author  :
  * @modification
  *	
  -----------------------------------------------------------------------------
**/ 
#ifndef __MENU_H__
#define __MENU_H__

#include <pthread.h>


int display_device_info_start(pthread_t *tid);
void *thread_display_device_info(void *arg);




#endif
