/**
  ******************************************************************************
  * @file    fifo.h
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.1
  * @date    2014-11-18
  * @brief
  ******************************************************************************
  * Change Logs:
  * Date           Author       Notes
  * 2014-11-18     ykaidong     the first version
  *
  ******************************************************************************
  * @attention
  *
  * Copyright(C) 2013-2014 by ykaidong<ykaidong@126.com>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU Lesser General Public License as
  * published by the Free Software Foundation; either version 2 of the
  * License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this program; if not, write to the
  * Free Software Foundation, Inc.,
  * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FIFO_H_
#define _FIFO_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint8_t *buffer;        //!< 缓冲区指针
    uint32_t size;          //!< 缓冲区大小
    uint32_t in;            //!< 写偏移
    uint32_t out;           //!< 读偏移
} fifo_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//! 创建一个名字为 FIFO_NAME 队列并初始化
//! 注意FIFO_BUF_SIZE 必须为2的整数次幂
 #define     CREATE_FIFO(FIFO_NAME, FIFO_BUF_SIZE)       \
     static uint8_t FIFO_NAME##_buf[FIFO_BUF_SIZE];      \
     static fifo_t FIFO_NAME = {                         \
         FIFO_NAME##_buf, FIFO_BUF_SIZE, 0, 0}

/* Exported functions ------------------------------------------------------- */
// fifo 初始化
extern void fifo_init(fifo_t *fifo);
// 向fifo 写入指定长度的数据
extern uint32_t fifo_put(fifo_t *fifo, uint8_t *buffer, uint32_t len);
// 从fifo 读出指定长度的数据
extern uint32_t fifo_get(fifo_t *fifo, uint8_t *buffer, uint32_t len);
// 向fifo 写入一字节数据
extern bool fifo_putc(fifo_t *fifo, uint8_t byte);
// 从fifo 读出一字节数据
extern bool fifo_getc(fifo_t *fifo, uint8_t *byte);

extern bool fifo_is_empty(fifo_t *fifo);
extern bool fifo_is_full(fifo_t *fifo);

#endif
/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


