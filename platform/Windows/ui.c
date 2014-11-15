/**
  ******************************************************************************
  * @file    ui.c
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.1
  * @date    2014-11-15
  * @brief   俄罗斯方块界面相关函数.
  * @note    俄罗斯方块的基本元素是一组由4个小型正方形组成的规则图形，
  *          英文称为Tetromino，中文通称为方块, 我们在这里称它为 <b> brick </b> ,
  *          共有7种，分别以S、Z、L、J、I、O、T这7个字母的形状来命名。
  *          我们将构成Tetromino的小正方形称为<b> box </b>.
  *
  *          经典的俄罗斯方块地图区由 10 * 20 个box组成.
  *          在本代码中, 我们设定地图的原点位于左上角, 坐标为(0, 0).
  ******************************************************************************
  * Change Logs:
  * Date           Author       Notes
  * 2014-11-15     ykaidong     the first version
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

#include "UI.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define     MAP_WIDTH           10      //!< 地图宽 10 个box
#define     MAP_HEIGHT          20      //!< 地图高 20 个box

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \brief 在地图区域或预览区域画一个box
 *
 * \param  x 地图区: 0 - 9
 * \param  y 地图区: 0 - 19
 */
void ui_draw_box(uint8_t x, uint8_t y, bool box)
{
    // 横向全角的一个方块符号占用两个字符位置
    gotoTextPos(x * 2 + 2, y + 1);

    if (!box)
        printf("□");
    else
        printf("■");

    return;
}



/**
 * \brief  打印分数
 *
 * \param  score
 */
void ui_print_score(uint32_t score)
{
    gotoTextPos(33, 16);
    printf("%4d", score);

    return;
}


/**
 * \brief  打印级别
 *
 * \param  level 1 - N
 */
void ui_print_level(uint8_t level)
{
    gotoTextPos(33, 10);
    printf("%4d", level);

    return;
}


/**
 * \brief  打印消行数
 *
 * \param  line
 */
void ui_print_line(uint8_t line)
{
    gotoTextPos(33, 13);
    printf("%4d", line);

    return;
}


/**
 * \brief  print game over
 */
void ui_print_game_over(void)
{
    gotoTextPos(4, 9);
    printf("                ");
    gotoTextPos(4, 10);
    printf("   GAME  OVER   ");
    gotoTextPos(4, 11);
    printf("                ");

    return;
}


/**
 * \brief  输入预览方块信息
 *
 * \param  brick
 */
void ui_print_preview(uint16_t brick)
{
    uint8_t x, y;
    bool bit;

    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            bit = ((brick & (0x0001 << (15 - (y * 4 + x)))) >> (15 - (y * 4 + x)));
            ui_draw_box(x + 13, y + 2, bit);
        }
    }

    return;
}




/**
 * \brief  print pause
 */
void ui_print_game_pause(void)
{
    gotoTextPos(4, 8);
    printf("                ");
    gotoTextPos(4, 9);
    printf("     PAUSE      ");
    gotoTextPos(4, 10);
    printf(" Press ENTER to ");
    gotoTextPos(4, 11);
    printf("    continue    ");
    gotoTextPos(4, 12);
    printf("                ");

    return;
}



void ui_init(void)
{
    fixConsoleSize(42, 22);
    setCursorVisible(0);
    setConsoleTitle("Tetris by DevLabs");

    gotoTextPos(0, 0);

    printf("┏━━━━━━━━━━┓              \n");
    printf("┃□□□□□□□□□□┃      Next    \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃    □□□□  \n");
    printf("┃□□□□□□□□□□┃    □□□□  \n");
    printf("┃□□□□□□□□□□┃    □□□□  \n");
    printf("┃□□□□□□□□□□┃    □□□□  \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃   级别:    1 \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃   消行:    0 \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃   分数:    0 \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┃□□□□□□□□□□┃   DevLabs.cn \n");
    printf("┃□□□□□□□□□□┃              \n");
    printf("┗━━━━━━━━━━┛                ");

    ui_print_line(0);
    ui_print_level(1);
    ui_print_score(0);
    ui_print_preview(0x0000);

    return;
}


/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


