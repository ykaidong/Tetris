/**
  ******************************************************************************
  * @file    term.h
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.1
  * @date    2014-11-22
  * @brief
  ******************************************************************************
  * Change Logs:
  * Date           Author       Notes
  * 2014-11-22     ykaidong     the first version
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
#ifndef _TERM_H_
#define _TERM_H_
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "uart.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
    black = 0,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white,
    defaults,
} color_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern void term_init(color_t foreground, color_t background);
// clear screen
extern void term_cls(void);
// put string
extern void term_puts(const char *str);
// save current cursor position
extern void term_save_cursor(void);
// restore cursor position after a Save Cursor
extern void term_restore_cursor(void);
// set cursor position
extern void term_set_cursor(uint8_t column, uint8_t row);
// set foreground color
extern void term_set_foreground(color_t color);
// set background color
extern void term_set_background(color_t color);


#endif
/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


