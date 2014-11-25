/**
  ******************************************************************************
  * @file    ui.c
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.1
  * @date    2014-11-23
  * @brief
  ******************************************************************************
  * Change Logs:
  * Date           Author       Notes
  * 2014-11-23     ykaidong     the first version
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
#include "ui.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UI_BG_COLOR                 white       // 背景色
#define UI_FG_COLOR                 black       // 前景色

#define MAP_BG_COLOR                magenta     // 地图背景色
#define PREVIEW_BG_COLOR            cyan        // 预览区背景色

#define BLOCK_COLOR                 blue        // 方块颜色

#define MAP_START_COLUMN            20          // 地图区域开始列
#define MAP_START_ROW               3           // 地图区域开始行

#define MAP_WIDTH                   (10 * 2)    // 一个方块占两个字符宽度
#define MAP_HEIGHT                  20          // 地图高

#define PREVIEW_START_COLUMN        45          // 预览区域开始列
#define PREVIEW_START_ROW           4           // 预览区域开始行

#define PREVIEW_WIDTH               13          // 预览区宽度
#define PREVIEW_HEIGHT              6           // 预览区高度

#define UI_TEXT_COLOR               red         // 文字颜色

/* Private macro -------------------------------------------------------------*/
#define RESET_CURSOR()              term_set_cursor(76, 24)
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \brief  word to ascii 双字节转ASCII
 *
 * \param  num 要转换的数字
 * \param  buf 字符串缓存区
 *
 * \return 转换后的字符串长度
 */
static uint8_t wtoa(uint16_t num, char *buf)
{
    char *p = buf;
    uint8_t tmp, len = 0;

    // 转换为ASCII
    while (num != 0)
    {
        *buf = num % 10 + '0';
        num /= 10;
        buf++;
    }

    // 字符串长度
    len = (uint8_t)(buf - p);

    // 字符串结束
    *buf = '\0';
    buf--;

    // 倒序
    while (p < buf)
    {
        tmp = *p;
        *p = *buf;
        *buf = tmp;
        p++;
        buf--;
    }

    return len;
}



/**
 * \brief  输出游戏级别
 *
 * \param  level
 */
void ui_print_level(uint8_t level)
{
    char level_buf[4];
    uint8_t i;

    i = wtoa(level, level_buf);
    term_save_cursor();
    term_set_cursor(55, 13);
    if (i == 1)
        term_puts("0");

    // 消除警告
    term_puts((const char *)level_buf);
    term_restore_cursor();

    return;
}



/**
 * \brief  输出消行行数
 *
 * \param  lines
 */
void ui_print_line(uint16_t line)
{
    char lines_buf[6];
    uint8_t i, j;

    term_save_cursor();
    term_set_cursor(54, 17);
    i = wtoa(line, lines_buf);
    // 前导0, 一共3位宽度
    for (j = 0; j < 3 - i; j++)
        term_puts("0");

    term_puts((const char *)lines_buf);
    term_restore_cursor();

    return;
}




/**
 * \brief  输出分数
 *
 * \param  score
 */
void ui_print_score(uint16_t score)
{
    char score_buf[6];
    uint8_t i, j;

    term_save_cursor();
    term_set_cursor(53, 21);
    i = wtoa(score, score_buf);
    // 一共4位宽度, 输出前导0
    for (j = 0; j < 4 - i; j++)
        term_puts("0");

    term_puts((const char *)score_buf);
    term_restore_cursor();

    return;
}



/**
 * \brief  在窗体上画一上box, 左上角坐标为(1, 1)
 *
 * \param  x
 * \param  y
 * \param  color
 */
static void draw_box(uint8_t x, uint8_t y, color_t color)
{
    term_set_background(color);
    term_set_cursor(x, y);
    term_puts("  ");
    // term_set_background(UI_BG_COLOR);

    return;
}

/**
 * \brief  在地图区域中画一个box
 *         原点位于左上角, 坐标为(0, 0)
 *
 * \param  x 地图x坐标
 * \param  y 地图y坐标
 * \param  box true时画一个box, false时擦除box
 */
void ui_draw_box(uint8_t x, uint8_t y, bool box)
{
    color_t c;
    if (box)
        c = BLOCK_COLOR;
    else
        c = MAP_BG_COLOR;
    // 一个box为两个字符宽度
    draw_box(x * 2 + MAP_START_COLUMN, y + MAP_START_ROW, c);
    // term_set_background(UI_BG_COLOR);

    return;
}

void ui_print_preview(uint16_t brick)
{
    uint8_t x, y;
    bool bit;

    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            bit = ((brick & (0x0001 << (15 - (y * 4 + x)))) >> (15 - (y * 4 + x)));

            if (bit)
                draw_box(x * 2 + PREVIEW_START_COLUMN + 1, y + PREVIEW_START_ROW + 1, BLOCK_COLOR);
            else
                draw_box(x * 2 + PREVIEW_START_COLUMN + 1, y + PREVIEW_START_ROW + 1, PREVIEW_BG_COLOR);
        }
    }

    term_set_background(UI_BG_COLOR);
    RESET_CURSOR();

    return;
}


/**
 * \brief  输出游戏结束信息
 */
void ui_print_game_over(void)
{
    term_set_cursor(22, 11);
    term_puts("                ");
    term_set_cursor(22, 12);
    term_puts("   GAME  OVER   ");
    term_set_cursor(22, 13);
    term_puts("                ");
    RESET_CURSOR();

    return;
}



/**
 * \brief  输出暂停信息
 */
void ui_print_game_pause(void)
{
    term_set_cursor(22, 10);
    term_puts("                ");
    term_set_cursor(22, 11);
    term_puts("   < PAUSE >    ");
    term_set_cursor(22, 12);
    term_puts(" Press ENTER to ");
    term_set_cursor(22, 13);
    term_puts("    continue    ");
    term_set_cursor(22, 14);
    term_puts("                ");
    RESET_CURSOR();

    return;
}




/**
 * \brief  UI init
 */
void ui_init(void)
{
    uint8_t i;

    term_init(UI_FG_COLOR, UI_BG_COLOR);

    // 画出地图区域
    term_set_background(MAP_BG_COLOR);
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        term_set_cursor(MAP_START_COLUMN, MAP_START_ROW + i);
        // 20 个空格
        term_puts("                    ");
    }

    // 画预览区域
    term_set_background(PREVIEW_BG_COLOR);
    for (i = 0; i < PREVIEW_HEIGHT; i++)
    {
        term_set_cursor(PREVIEW_START_COLUMN, PREVIEW_START_ROW + i);
        // 13个空格
        term_puts("             ");
    }

    // 恢复原来的背景
    term_set_background(UI_BG_COLOR);

    // 设置UI文字颜色
    term_set_foreground(UI_TEXT_COLOR);
    // Next
    term_set_cursor(49, 3);
    term_puts("NEXT");

    // level
    term_set_cursor(45, 12);
    term_puts("+------------+");
    term_set_cursor(45, 13);
    term_puts("+ Level:     +");
    term_set_cursor(45, 14);
    term_puts("+------------+");

    // line
    term_set_cursor(45, 16);
    term_puts("+------------+");
    term_set_cursor(45, 17);
    term_puts("+ Lines:     +");
    term_set_cursor(45, 18);
    term_puts("+------------+");

    // score
    term_set_cursor(45, 20);
    term_puts("+------------+");
    term_set_cursor(45, 21);
    term_puts("+ Score:     +");
    term_set_cursor(45, 22);
    term_puts("+------------+");
    // info
    term_set_cursor(55, 24);
    term_puts("http://www.DevLabs.cn");

    ui_print_level(1);
    ui_print_line(0);
    ui_print_score(0);

    RESET_CURSOR();

    return;
}


/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


