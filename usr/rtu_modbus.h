/**
  -----------------------------------------------------------------------------
  * @file   : rtu_modbus.h
  * @author : xp
  * @version: V0.0.1
  * @date   : 2020/09/12
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
  
#ifndef __RTU_MODBUS_H
#define __RTU_MODBUS_H

#include <pthread.h>

/*常用功能码*/
#define MB_ADDRESS_BROADCAST    ( 0 )   /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN          ( 1 )   /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX          ( 247 ) /*! Biggest possible slave address. */
#define MB_FUNC_NONE                          (  0 )  //无效功能码
#define MB_FUNC_READ_COILS                    (  1 )  //0x01  读线圈
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )  //0x02  读输入离散量
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )  //0x05  写单个线圈
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )  //0x0F  写多个线圈
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )  //0x03  读多个寄存器
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )  //0x04  读输入寄存器
#define MB_FUNC_WRITE_REGISTER                (  6 )  //0x06  写单个寄存器
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )  //0x10  写多个寄存器
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 23 )  //0x17  读/写多个寄存器
#define MB_FUNC_ERROR                         ( 128 ) //0x80  错误功能码


/*!
 * Constants which defines the format of a modbus frame. The example is
 * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
 * dependent on the underlying transport.
 *
 * <code>
 * <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       | CRC/LRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MB_SER_PDU_SIZE_MAX = 256
 * (2)  ... MB_SER_PDU_ADDR_OFF = 0
 * (3)  ... MB_SER_PDU_PDU_OFF  = 1
 * (4)  ... MB_SER_PDU_SIZE_CRC = 2
 *
 * (1') ... MB_PDU_SIZE_MAX     = 253
 * (2') ... MB_PDU_FUNC_OFF     = 0
 * (3') ... MB_PDU_DATA_OFF     = 1
 * </code>
 */

#define RTU_SLAVE_ADDR     1

/*RTU MODBUS使能*/
#define MB_RTU_ENABLED                          (0)

/*TCP MODBUS使能*/
#define MB_TCP_ENABLED                          (1)


#define MB_FUNC_HANDLERS_MAX                    (8)

#define MB_FUNC_READ_INPUT_ENABLED              (0)
#define MB_FUNC_READ_HOLDING_ENABLED            (1)
#define MB_FUNC_WRITE_HOLDING_ENABLED           (0)
#define MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  (0)
#define MB_FUNC_READ_COILS_ENABLED              (0)
#define MB_FUNC_WRITE_COIL_ENABLED              (0)
#define MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED    (0)
#define MB_FUNC_READWRITE_HOLDING_ENABLED       (0)


/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_SIZE_MAX     253 /*!< Maximum size of a PDU. */
#define MB_PDU_SIZE_MIN     1   /*!< Function Code */
#define MB_PDU_FUNC_OFF     0   /*!< Offset of function code in PDU. */
#define MB_PDU_DATA_OFF     1   /*!< Offset for response data in PDU. */


typedef enum
{
	rx_status = 0,
	tx_status = 1
} eRTUbusStatus;

typedef enum
{
	MB_EX_NONE  = 0x00,
  MB_EX_ILLEGAL_FUNCTION     = 0x01,
  MB_EX_ILLEGAL_DATA_ADDRESS = 0x02,
  MB_EX_ILLEGAL_DATA_VALUE   = 0x03,
  MB_EX_SLAVE_DEVICE_FAILURE = 0x04,
  MB_EX_ACKNOWLEDGE   = 0x05,
  MB_EX_SLAVE_BUSY    = 0x06,
  MB_EX_MEMORY_PARITY_ERROR = 0x08,
  MB_EX_GATEWAY_PATH_FAILED = 0x0A,
  MB_EX_GATEWAY_TGT_FAILED  = 0x0B
} eMBException;

typedef enum
{
	MB_ENOERR = 0,           /*!< no error. */
	MB_ENOREG,           /*!< illegal register address. */
	MB_EINVAL,           /*!< illegal argument. */
	MB_EPORTERR,         /*!< porting layer error. */
	MB_ENORES,           /*!< insufficient resources. */
	MB_EIO,              /*!< I/O error. */
	MB_EILLSTATE,        /*!< protocol stack in illegal state. */
	MB_ETIMEDOUT         /*!< timeout error occurred. */
} eMBErrorCode;


int rtu_modbus_poll_start(pthread_t *tid);
void *thread_modbus_poll(void *arg);


#endif /* __RTU_MODBUS_H */

/*---------------------------------The End------------------------------------*/
