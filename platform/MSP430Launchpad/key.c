/**
  ******************************************************************************
  * @file    key.c
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
#include "key.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void key_init(void)
{
    return;
}

// up       : 0x1B, 0x5B, 0x41                  // <ESC>[A
// down     : 0x1B, 0x5B, 0x42                  // <ESC>[B
// right    : 0x1B, 0x5B, 0x43                  // <ESC>[C
// left     : 0x1B, 0x5B, 0x44                  // <ESC>[D
// home     : 0x1B, 0x5B, 0x31, 0x7E            // <ESC>[1~
// insert   : 0x1B, 0x5B, 0x32, 0x7E            // <ESC>[2~
// delete   : 0x1B, 0x5B, 0x33, 0x7E            // <ESC>[3~
// end      : 0x1B, 0x5B, 0x34, 0x7E            // <ESC>[4~
// pageup   : 0x1B, 0x5B, 0x35, 0x7E            // <ESC>[5~
// pagedown : 0x1B, 0x5B, 0x36, 0x7E            // <ESC>[6~
// F1       : 0x1B, 0x5B, 0x31, 0x31, 0x7E      // <ESC>[11~
// F2       : 0x1B, 0x5B, 0x31, 0x32, 0x7E      // <ESC>[12~
// F3       : 0x1B, 0x5B, 0x31, 0x33, 0x7E      // <ESC>[13~
// F4       : 0x1B, 0x5B, 0x31, 0x34, 0x7E      // <ESC>[14~
// F5       : 0x1B, 0x5B, 0x31, 0x35, 0x7E      // <ESC>[15~
// F6       : 0x1B, 0x5B, 0x31, 0x37, 0x7E      // <ESC>[17~
// F7       : 0x1B, 0x5B, 0x31, 0x38, 0x7E      // <ESC>[18~
// F8       : 0x1B, 0x5B, 0x31, 0x39, 0x7E      // <ESC>[19~
// F9       : 0x1B, 0x5B, 0x32, 0x30, 0x7E      // <ESC>[20~
// F10      : 0x1B, 0x5B, 0x32, 0x31, 0x7E      // <ESC>[21~
// F11      : 0x1B, 0x5B, 0x32, 0x33, 0x7E      // <ESC>[23~
// F12      : 0x1B, 0x5B, 0x32, 0x34, 0x7E      // <ESC>[24~

#define KEY_LEAD1           0x1B
#define KEY_LEAD2           0x5B
#define KEY_UP              0x41
#define KEY_DOWN            0x42
#define KEY_RIGHT           0x43
#define KEY_LEFT            0x44

#define KEY_WAIT_LEAD1      0x00
#define KEY_WAIT_LEAD2      0x01
#define KEY_STATE_VERIFY    0x02

key_t key_get(void)
{
    static uint8_t state;
    uint8_t data = 0;
    key_t key = key_null;

    switch (state)
    {
    case KEY_WAIT_LEAD1:
        // 有键按下
        if (uart_getc(&data))
        {
            // 非引导键, 直接返回键值
            if (data != KEY_LEAD1)
            {
                key = (key_t)data;
                break;
            }
            else
            {
                state = KEY_WAIT_LEAD2;
            }
        }
        else
        {
            break;
        }
    case KEY_WAIT_LEAD2:
        if (uart_getc(&data))
        {
            // 得到第二个引导码, 转下一状态
            if (data == KEY_LEAD2)
            {
                state = KEY_STATE_VERIFY;
            }
            else
            {
                // 错误, 转第一状态
                state = KEY_WAIT_LEAD1;
                break;
            }
        }
        else
        {
            // 无输入, 在本状态等候
            break;
        }
    case KEY_STATE_VERIFY:
        if (uart_getc(&data))
        {
            if (data == KEY_UP)
                key = key_up;
            else if (data == KEY_DOWN)
                key = key_down;
            else if (data == KEY_LEFT)
                key = key_left;
            else if (data == KEY_RIGHT)
                key = key_right;
            else
                key = key_null;
            state = KEY_WAIT_LEAD1;
        }
        break;
    default:
        break;
    }

    return key;
}
/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


