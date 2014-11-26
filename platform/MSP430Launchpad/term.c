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
#include <string.h>             // strcpy()
#include "term.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define     TERM_PUTC(c)        uart_putc(c)

#define     RESET               0
#define     CLEAR               1
#define     SAVE_CURSOR         2
#define     RESTORE_CURSOR      3
#define     SET_FG_COLOR        4
#define     SET_BG_COLOR        5
#define     SET_COUSOR_POS      6

/* Private variables ---------------------------------------------------------*/
// foreground and background, default bg = black, fg = white
static color_t color_fg = white, color_bg = black;

static const char *ctrl_set[] =
{
    "\033c",        // <ESC>c, reset
    "\033[2J",      // clear screen
    "\033[s",       // save cursor postion
    "\033[u",       // restore cursor
    "\033[1;37m",   // foreground, bright white, 3x
    "\033[1;40m",   // background, bright black, 4x
    "\033[01;01H",  // cursor, x: 01, y:01
                    // the position of home is (1, 1)
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \brief  reset terminal
 */
void term_reset(void)
{
    // reset, command: <ESC>c
    term_puts(ctrl_set[RESET]);

    return;
}

/**
 * \brief  Erases the screen with the background colour
 *         and moves the cursor to home
 */
void term_cls(void)
{
    term_puts(ctrl_set[CLEAR]);

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
    term_set_foreground(color_fg);
    term_set_background(color_bg);
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
    char buffer[9];

    // <ESC>[01;01H
    strcpy(buffer, ctrl_set[SET_COUSOR_POS]);
    buffer[2] = (row / 10 % 10 + '0');
    buffer[3] = (row % 10 + '0');
    buffer[5] = (column / 10 % 10 + '0');
    buffer[6] = (column % 10 + '0');
    term_puts((const char *)buffer);

    return;
}




/**
 * \brief  set foreground color
 *
 * \param  color
 */
void term_set_foreground(color_t color)
{
    char buffer[8];

    if (color != defaults)
    {
        strcpy(buffer, ctrl_set[SET_FG_COLOR]);
        // <ESC>[1;37m
        //          ^
        //         color
        buffer[5] = (uint8_t)color % 10 + '0';
        term_puts((const char *)buffer);
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
    char buffer[8];

    if (color != defaults)
    {
        strcpy(buffer, ctrl_set[SET_BG_COLOR]);
        buffer[5] = (uint8_t)color % 10 + '0';
        term_puts((const char *)buffer);
    }

    return;
}

/**
 * \brief  put string
 *
 * \param  color color for string
 * \param  str
 */
void term_puts(const char *str)
{
    while (*str != '\0')
    {
        // XXX
        // 此处使用等待的方式发送, 数据量大时可能会在此处卡顿
        // 待改进
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
    term_puts(ctrl_set[SAVE_CURSOR]);

    return;
}


/**
 * \brief  Restores cursor position after a save cursor
 */
void term_restore_cursor(void)
{
    term_puts(ctrl_set[RESTORE_CURSOR]);

    return;
}

/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


