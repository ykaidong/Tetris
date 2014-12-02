/**
  ******************************************************************************
  * @file    ui.c
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.3
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
  * 2014-11-23     ykaidong     将画box的函数独立出来
  * 2014-11-30     ykaidong     支持彩色
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
#define     MAP_WIDTH               10      //!< 地图宽 10 个box
#define     MAP_HEIGHT              20      //!< 地图高 20 个box

#define     MAP_START_COLUMN        2
#define     MAP_START_ROW           1

#define     PREVIEW_START_COLUMN    28
#define     PREVIEW_START_ROW       3

#define     UI_COLOR_BASE           8       //!< 亮色, 参考pcc32.h
#define     UI_BG_COLOR             WHITE   //!< 地图背景色, 非控制台背景

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \brief  在窗体上画一个box, 左上角坐标为(0, 0)
 *
 * \param  x
 * \param  y
 * \param  color 颜色, 0 - 7. 注意0表示清除该box, 非颜色
 */
static void draw_box(uint8_t x, uint8_t y, uint8_t color)
{
    gotoTextPos(x, y);

    // 清除
    if (!color)
    {
        setTextColor(UI_BG_COLOR);
        printf("□");
    }
    else
    {
#ifdef UI_USE_COLOR
        // 从UI_COLOR_BASE起是亮色
        setTextColor(color + UI_COLOR_BASE);
#else
        // 单色模式下, 设前景为白色
        setTextColor(WHITE);
#endif
        printf("■");
    }

    return;
}


/**
 * \brief 在地图区域画一个box
 *
 * \param  x 地图区: 0 - 9
 * \param  y 地图区: 0 - 19
 */
void ui_draw_box(uint8_t x, uint8_t y, uint8_t color)
{
    // 横向全角的一个方块符号占用两个字符位置
    draw_box(x * 2 + MAP_START_COLUMN, y + MAP_START_ROW, color);

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
    setTextColor(WHITE);
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
    setTextColor(WHITE);
    printf("%4d", level);

    return;
}


/**
 * \brief  打印消行数
 *
 * \param  line
 */
void ui_print_line(uint16_t line)
{
    gotoTextPos(33, 13);
    setTextColor(WHITE);
    printf("%4d", line);

    return;
}


/**
 * \brief  print game over
 */
void ui_print_game_over(void)
{
    setTextColor(WHITE);
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
void ui_print_preview(const void *info)
{
    bool bit;
    uint8_t x, y;
    uint16_t brick;

    const uint8_t *p_info = info;
    // 取得方块数据
    brick = (*(uint16_t *)p_info);

#ifdef UI_USE_COLOR
    uint8_t color;
    // ASCII C 中不允许对 void 型指针进行运算
    p_info += 2;
    // 得到颜色值
    color = (*(uint8_t *)p_info);
#endif

    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            bit = ((brick & (0x0001 << (15 - (y * 4 + x)))) >> (15 - (y * 4 + x)));
            // 横向全角的一个方块符号占用两个字符位置
#ifdef UI_USE_COLOR
            if (bit)
                draw_box(x * 2 + PREVIEW_START_COLUMN, y + PREVIEW_START_ROW, color);
            else
                draw_box(x * 2 + PREVIEW_START_COLUMN, y + PREVIEW_START_ROW, 0);
#else
            draw_box(x * 2 + PREVIEW_START_COLUMN, y + PREVIEW_START_ROW, bit);
#endif
        }
    }

    return;
}




/**
 * \brief  print pause
 */
void ui_print_game_pause(void)
{
    setTextColor(WHITE);
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

    setTextColor(WHITE);
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

    // 预览方块信息
    uint32_t temp = 0;
    ui_print_preview(&temp);

    return;
}


/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


