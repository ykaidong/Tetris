/**
  ******************************************************************************
  * @file    fifo.c
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>         // memcpy()
#include "fifo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define     MIN(a, b)   ((a) < (b) ? (a) : (b))
#define     MAX(a, b)   ((a) < (b) ? (b) : (a))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \brief  判断一个数是否为2的整数次幂
 *
 * \param  n 要判断的数
 *
 * \retval true
 * \retval false
 */
static bool is_pow_of_2(uint32_t x)
{
    return !(x & (x-1));
}



void fifo_init(fifo_t *fifo)
{
    // assert_param(!is_pow_of_2(fifo->size));

    // XXX
    // buffer大小必须为2的整数次幂
    if (!is_pow_of_2(fifo->size))
        while (1);

    fifo->in = fifo->out = 0;

    return;
}

/**
 * \brief  将一个buffer写入FIFO
 *
 * \param  fifo   要写入的FIFO
 * \param  buffer 缓存指针
 * \param  len    要写入的长度
 *
 * \return 实际写入的长度
 */
uint32_t fifo_put(fifo_t *fifo, uint8_t *buffer, uint32_t len)
{
    uint32_t l;

    len = MIN(len, fifo->size - (fifo->in - fifo->out));
    l = MIN(len, fifo->size - (fifo->in & (fifo->size - 1)));

    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
    memcpy(fifo->buffer, buffer + l, len - l);

    fifo->in += len;

    return len;
}

/**
 * \brief  写入一个字节的数据到FIFO
 *
 * \param  fifo 待写入的FIFO
 * \param  c    待写入的字符
 *
 * \retval true
 * \retval false
 */
bool fifo_putc(fifo_t *fifo, uint8_t byte)
{
    // fifo已满
    if (fifo->size - (fifo->in - fifo->out) == 0)
        return false;

    *(fifo->buffer + (fifo->in & (fifo->size - 1))) = byte;
    fifo->in++;

    return true;
}


/**
 * \brief  从fifo中取出数据
 *
 * \param  fifo   待操作的fifo
 * \param  buffer 目标缓存
 * \param  len    待取出的数据
 *
 * \return 实际取出的数据
 */
uint32_t fifo_get(fifo_t *fifo, uint8_t *buffer, uint32_t len)
{
    uint32_t l;

    len = MIN(len, fifo->in - fifo->out);
    l = MIN(len, fifo->size - (fifo->out & (fifo->size - 1)));

    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
    memcpy(buffer + l, fifo->buffer, len - l);

    fifo->out += len;

    return len;
}



/**
 * \brief  从FIFO中取出一个字节的数据
 *
 * \param  fifo 目标FIFO
 * \param  byte
 *
 * \retval true
 * \retval false
 */
bool fifo_getc(fifo_t *fifo, uint8_t *byte)
{
    // fifo为空
    if (fifo->in == fifo->out)
        return false;

    *byte = *(fifo->buffer + (fifo->out & (fifo->size - 1)));
    fifo->out++;

    return true;
}



/**
 * \brief  fifo is empty?
 *
 * \param  fifo
 */
bool fifo_is_empty(fifo_t *fifo)
{
    return (fifo->in == fifo->out);
}


/**
 * \brief  fifo is full?
 *
 * \param  fifo
 */
bool fifo_is_full(fifo_t *fifo)
{
    return (fifo->out + fifo->size == fifo->in);
}





/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


