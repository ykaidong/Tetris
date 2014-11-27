/**
  ******************************************************************************
  * @file    main.c
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.2
  * @date    2014-11-15
  * @brief
  ******************************************************************************
  * Change Logs:
  * Date           Author       Notes
  * 2014-11-15     ykaidong     the first version
  * 2014-11-27     ykaidong     更新接口后重写部分代码
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
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "ui.h"
#include "Tetris.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static bool pause = false;          // 游戏暂停
static uint8_t level = 1;           // 级别
static uint16_t lines = 0;          // 消除的行数
static uint16_t score = 0;          // 分数
static uint16_t time_count = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void game_over(void)
{
    ui_print_game_over();

    return;
}


/**
 * \brief  更新分数等
 */
void game_info_update(void)
{
    ui_print_level(level);
    ui_print_line(lines);
    ui_print_score(score);

    return;
}


/**
 * \brief  返回预览方块(下一个方块)的信息
 *
 * \param  info 16位整数, 从bit 15 开始
 *         每四位为一组, 组成一个4*4的点阵
 *         点阵中为1的位组成下一个方块的图形
 */
void get_preview_brick(const void *info)
{
    uint16_t dat = *((uint16_t *)info);

    ui_print_preview(dat);

    return;
}


/**
 * \brief  在地图上画一个box
 *
 * \param  x
 * \param  y
 * \param  color 为0时清楚此坐标上的box
 */
void draw_box(uint8_t x, uint8_t y, uint8_t color)
{
    if (color == 0)
        ui_draw_box(x, y, false);
    else
        ui_draw_box(x, y, true);

    return;
}


/**
 * \brief  Tetris模块获得随机数的回调函数
 *
 * \return 产生的随机数
 */
uint8_t random_num(void)
{
    return (uint8_t)rand();
}


/**
 * \brief  Tetris返回消除行数的回调函数
 *
 * \param  line 消行数
 */
void get_remove_line_num(uint8_t line)
{
    lines += line;

    switch (line)
    {
    case 1:
        score += 10;
        break;
    case 2:
        score += 25;
        break;
    case 3:
        score += 45;
        break;
    case 4:
        score += 80;
        break;
    default:
        break;
    }

    // 每25行升一级
    level = lines / 25 + 1;

    return;
}


/**
 * \brief  游戏暂停
 */
void game_pause(void)
{
    pause = !pause;

    if (pause)
        ui_print_game_pause();
    else
        tetris_sync_all();      // 因为打印暂停破坏了地图区显示
                                // 所以退出时要刷新整个地图区
    return;
}


void game_run(void)
{
    uint16_t key;
    static bool refresh = false;

    delayMS(50);

    if (!pause)
        time_count++;

    // 级别越高速度越快
    if (time_count >= (12 - level))
    {
        time_count = 0;

        refresh = true;
        tetris_move(dire_down);
    }

    if (kbhit())
    {
        key = jkGetKey();

        // 暂停时只响应回车键
        if (pause && key != JK_ENTER)
            return;

        switch (key)
        {
        case JK_UP:
            tetris_move(dire_rotate);
            break;
        case JK_DOWN:
            tetris_move(dire_down);
            break;
        case JK_LEFT:
            tetris_move(dire_left);
            break;
        case JK_RIGHT:
            tetris_move(dire_right);
            break;
        case JK_SPACE:
            while (tetris_move(dire_down));
            break;
        case JK_ENTER:
            game_pause();
            break;
        default:
            break;
        }

        refresh = true;
        fflush(stdin);
    }

    if (refresh)
    {
        refresh = !refresh;
        tetris_sync();
        // 更新行数, 分数等信息
        game_info_update();
    }

    return;
}


int main(void)
{
    // 随机数种子
    srand((int32_t)time(NULL));

    ui_init();
    tetris_init(&draw_box, &random_num, &get_preview_brick, &get_remove_line_num);

    game_pause();

    while (!tetris_is_game_over())
    {
        game_run();
    }

    game_over();

    // 按回车退出
    while (getch() != 13);

    return 0;
}


/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


