/**
  ******************************************************************************
  * @file    uart.c
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
#include <string.h>         // strlen()
#include "uart.h"
#include "fifo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UART_TX_FIFO_SIZE       256
#define UART_RX_FIFO_SIZE       32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CREATE_FIFO(uart_tx_fifo, UART_TX_FIFO_SIZE);
CREATE_FIFO(uart_rx_fifo, UART_RX_FIFO_SIZE);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  uart init
 */
void uart_init(void)
{
    // IO init
    P1SEL = BIT1 + BIT2 ;                   // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                  // P1.1 = RXD, P1.2=TXD

    UCA0CTL1 |= UCSSEL_2;                   // SMCLK

    UCA0BR0 = 8;
    UCA0BR1 = 0;
    UCA0MCTL |= UCBRS_6;

    UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                        // Enable USCI_A0 RX interrupt

    fifo_init(&uart_tx_fifo);
    fifo_init(&uart_rx_fifo);
}


/**
 * @brief  uart put a char
 *
 * @param  ch
 *
 * @retern true/false
 */
bool uart_putc(char ch)
{
    bool flg = false;

    flg = fifo_putc(&uart_tx_fifo, ch);
    IE2 |= UCA0TXIE;

    return flg;
}


/**
 * @brief  uart get a char
 *
 * @param  byte
 *
 * @return true/false
 */
bool uart_getc(uint8_t *byte)
{
    return fifo_getc(&uart_rx_fifo, byte);
}



/**
 * @brief  发送一个字符串
 *
 * @param  str
 *
 * @return 实际发送的字符数
 */
uint8_t uart_puts(uint8_t *str)
{
    uint8_t i;

    i = fifo_put(&uart_tx_fifo, str, strlen((const char *)str));
    IE2 |= UCA0TXIE;

    return i;
}


//  Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    // while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
    // UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
    fifo_putc(&uart_rx_fifo, UCA0RXBUF);
}


#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    // 如果fifo为空, 关中断
    if (fifo_is_empty(&uart_tx_fifo))
        IE2 &= ~UCA0TXIE;
    else
        fifo_getc(&uart_tx_fifo, (uint8_t *)&UCA0TXBUF);
}


/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


