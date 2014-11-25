/**
  ******************************************************************************
  * @file    ui.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UI_H_
#define _UI_H_
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "term.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void ui_init(void);
extern void ui_draw_box(uint8_t x, uint8_t y, bool box);
extern void ui_print_preview(uint16_t brick);
extern void ui_print_level(uint8_t level);
extern void ui_print_line(uint16_t line);
extern void ui_print_score(uint16_t score);
extern void ui_print_game_over(void);
extern void ui_print_game_pause(void);

#endif
/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


