/**
  ******************************************************************************
  * @file    main.c
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.2
  * @date    2014-11-17
  * @brief
  ******************************************************************************
  * Change Logs:
  * Date           Author       Notes
  * 2014-11-17     ykaidong     the first version
  * 2014-11-27     ykaidong     更新tetris.h的接口后添加必要的代码
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
#include <MSP430.h>
#include <stdbool.h>
#include <stdint.h>
#include "tetris.h"
#include "uart.h"
#include "key.h"
#include "ui.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define LED_INIT()          do {P1DIR |= BIT0; P1OUT &= ~BIT0;} while (0)
#define LED_ON()            do {P1OUT |= BIT0; } while (0)
#define LED_OFF()           do {P1OUT &= ~BIT0;} while (0)
#define LED_TRIGGER()       do {if(P1IN & 0x01) P1OUT &= ~BIT0;\
                                else P1OUT |= BIT0;} while (0)

/* Private variables ---------------------------------------------------------*/
static volatile uint16_t timer_count;

static bool pause = false;          // 游戏暂停
static uint8_t level = 1;           // 级别
static uint16_t lines = 0;          // 消除的行数
static uint16_t score = 0;          // 分数
static uint16_t speed_count = 0;

/* Private function prototypes -----------------------------------------------*/
static void timer_init(void);
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
    uint16_t dat = *(uint16_t *)info;

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
    // XXX
    // 随机数产生, 直接读定时器值, 待改进
    return (uint8_t)TAR % 7;
}


/**
 * \brief  Tetris返回消除行数的回调函数
 *
 * \param  line 消行数
 */
void get_remove_line_num(uint8_t line)
{
    static uint16_t count;

    lines += line;
    count++;

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

    // 每消行20次升一级
    level = lines / 20 + 1;

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
    static bool refresh = false;

    if (!pause)
        speed_count++;

    // 级别越高速度越快
    if (speed_count >= (12 - level))
    {
        speed_count = 0;

        refresh = true;
        tetris_move(dire_down);
    }

    key_t key = key_get();
    if (key != key_null)
    {
        // 暂停时只响应回车键
        if (pause && key != key_enter)
            return;

        switch (key)
        {
        case key_up:
            tetris_move(dire_rotate);
            break;
        case key_down:
            tetris_move(dire_down);
            break;
        case key_left:
            tetris_move(dire_left);
            break;
        case key_right:
            tetris_move(dire_right);
            break;
        case key_space:
            while (tetris_move(dire_down));
            break;
        case key_enter:
            game_pause();
            break;
        default:
            break;
        }

        refresh = true;
    }

    if (refresh)
    {
        refresh = !refresh;
        tetris_sync();
        // 更新行数, 分数等信息
        game_info_update();
        ui_reset_cursor();
        LED_TRIGGER();
    }

    return;
}


int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    if (CALBC1_16MHZ == 0xFF)                   // If calibration constant erased
    {
        while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                                 // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_16MHZ;                     // Set range
    DCOCTL = CALDCO_16MHZ;                      // Set DCO step + modulation

    // P1DIR |= BIT4;                              // P1.4输出SMCLK.
    // P1SEL |= BIT4;

    LED_INIT();
    timer_init();
    uart_init();
    key_init();

    __bis_SR_register(GIE);         // 开全局中断

    ui_init();
    tetris_init(&draw_box, &random_num, &get_preview_brick, &get_remove_line_num);

    game_pause();

    while (1)
    {
        if (!tetris_is_game_over())
        {
            if (timer_count > 50)
            {
                timer_count = 0;
                game_run();
            }
        }
        else
        {
            // XXX
            game_over();
            __bis_SR_register(LPM0_bits);       // Enter LPM0
            while (1);
        }

        __bis_SR_register(LPM0_bits);       // Enter LPM0
    }
    // return 0;
}



void timer_init(void)
{
    TACCR0 = 16000;
    TACTL |= TASSEL_2 + MC_1;               // SMCLK, up mode->counts up to TACCR0
    TACTL |= TAIE;                          // Enable interrupt

    return;
}


#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    switch(TA0IV)
    {
    case 2: break;              // CCR1 not used
    case 4: break;              // CCR2 not used
    case 10:                    // overflow
        timer_count++;
        break;
    }

    LPM0_EXIT;                  // exit low power mode
}


/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


