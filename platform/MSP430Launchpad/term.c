/**
  ******************************************************************************
  * @file    term.c
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

/* Includes ------------------------------------------------------------------*/
#include "term.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define     ESC                 0x1B
/* Private macro -------------------------------------------------------------*/
#define     TERM_PUTC(c)        uart_putc(c)

/* Private variables ---------------------------------------------------------*/
// foreground and background, default bg = black, fg = white
static color_t color_fg = white, color_bg = black;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \brief  reset terminal
 */
void term_reset(void)
{
    // reset, command: <ESC>c
    TERM_PUTC(ESC);
    TERM_PUTC('c');

    return;
}

/**
 * \brief  Erases the screen with the background colour
 *         and moves the cursor to home
 */
void term_cls(void)
{
    TERM_PUTC(ESC);
    TERM_PUTC('[');
    TERM_PUTC('2');
    TERM_PUTC('J');

    return;
}

/**
 * \brief  init terminal
 *
 * \param  foreground foreground color, Bright.
 * \param  background background color, Bright.
 */
void term_init(color_t foreground, color_t background)
{
    // 不可为defaults
    if (foreground != defaults && background != defaults)
    {
        color_fg = foreground;
        color_bg = background;
    }

    term_reset();

    // Set background and foreground
    // command: <ESC>[{attr1};...;{attrn}m
    TERM_PUTC(ESC);
    TERM_PUTC('[');
    TERM_PUTC('1');     // Bright color
    TERM_PUTC(';');
    TERM_PUTC('3');     // foreground, 3x
    TERM_PUTC(color_fg % 10 + '0');
    TERM_PUTC(';');
    TERM_PUTC('4');
    TERM_PUTC(color_bg % 10 + '0');
    TERM_PUTC('m');

    term_cls();

    return;
}


/**
 * \brief  Set cursor Position
 *
 * \param  row
 * \param  column
 */
void term_set_cursor(uint8_t column, uint8_t row)
{
    // command: <ESC>[{row};{column}H
    // the position of home is (1, 1)
    TERM_PUTC(ESC);
    TERM_PUTC('[');
    TERM_PUTC(row / 10 % 10 + '0');
    TERM_PUTC(row % 10 + '0');
    TERM_PUTC(';');
    TERM_PUTC(column / 10 % 10 + '0');
    TERM_PUTC(column % 10 + '0');
    TERM_PUTC('H');

    return;
}




/**
 * \brief  set foreground color
 *
 * \param  color
 */
void term_set_foreground(color_t color)
{
    if (color != defaults)
    {
        // foreground: 3x
        TERM_PUTC(ESC);
        TERM_PUTC('[');
        TERM_PUTC('3');
        TERM_PUTC(color % 10 + '0');
        TERM_PUTC('m');
    }

    return;
}

/**
 * \brief  Set background color
 *
 * \param  color
 */
void term_set_background(color_t color)
{
    if (color != defaults)
    {
        // background: 4x
        TERM_PUTC(ESC);
        TERM_PUTC('[');
        TERM_PUTC('4');
        TERM_PUTC(color % 10 + '0');
        TERM_PUTC('m');
    }

    return;
}

/**
 * \brief  put string
 *
 * \param  color color for string
 * \param  str
 */
void term_puts(uint8_t *str)
{
    while (*str != '\0')
    {
        while (!TERM_PUTC(*str));
        str++;
    }

    return;
}


/**
 * \brief  save current corsor postion
 */
void term_save_cursor(void)
{
    TERM_PUTC(ESC);
    TERM_PUTC('[');
    TERM_PUTC('s');

    return;
}


/**
 * \brief  Restores cursor position after a save cursor
 */
void term_restore_cursor(void)
{
    TERM_PUTC(ESC);
    TERM_PUTC('[');
    TERM_PUTC('u');

    return;
}

/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


