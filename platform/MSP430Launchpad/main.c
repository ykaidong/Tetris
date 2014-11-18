/**
  ******************************************************************************
  * @file    main.c
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.1
  * @date    2014-11-17
  * @brief
  ******************************************************************************
  * Change Logs:
  * Date           Author       Notes
  * 2014-11-17     ykaidong     the first version
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
#include <MSP430.h>
#include <stdbool.h>
#include <stdint.h>
#include "uart.h"

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

/* Private function prototypes -----------------------------------------------*/
void timer_init(void);

/* Private functions ---------------------------------------------------------*/
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    LED_INIT();
    timer_init();
    uart_init();

    __bis_SR_register(GIE);         // 开全局中断

    uart_puts("hello launchpad!\n");

    while (1)
    {
        uint8_t i;
        if (uart_getc(&i))
            uart_putc(i);

        if (timer_count > 500)
        {
            timer_count = 0;
            LED_TRIGGER();
        }

        __bis_SR_register(LPM0_bits);       // Enter LPM0
    }
    // return 0;
}



void timer_init(void)
{
    TACCR0 = 1000;                          // 1ms@1MHz
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


