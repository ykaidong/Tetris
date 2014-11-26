/**
  ******************************************************************************
  * @file    Tetris.c
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.1
  * @date    2014-11-15
  * @brief
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

/* Includes ------------------------------------------------------------------*/
#include "tetris.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    int8_t x;               //!< brick在地图中的x坐标
    int8_t y;               //!< brick在地图中的y坐标
    int8_t index;           //!< 方块索引, 高4位记录类型, 低4位记录变形
    uint16_t brick;         //!< 方块数据
} brick_t;

/* Private define ------------------------------------------------------------*/
#define BRICK_TYPE                  7   // 一共7种类型的方块
#define BRICK_NUM_OF_TYPE           4   // 每一种类型有4种变形

#define BRICK_HEIGHT                4   // 一个brick由4*4的box组成
#define BRICK_WIDTH                 4

#define TETRIS_MAP_WIDTH            10  // 地图宽
#define TETRIS_MAP_HEIGHT           20  // 地图高

#define BRICK_START_X               ((TETRIS_MAP_WIDTH / 2) - (BRICK_WIDTH / 2))

#ifndef NULL
    #define NULL    ((void *)0)
#endif

/* Private macro -------------------------------------------------------------*/
// 对地图数组进行位操作的支持宏
#define     SET_BIT(dat, bit)      ((dat) |= (0x0001 << (bit)))
#define     CLR_BIT(dat, bit)      ((dat) &= ~(0x0001 << (bit)))
#define     GET_BIT(dat, bit)      (((dat) & (0x0001 << (bit))) >> (bit))

/* Private variables ---------------------------------------------------------*/
// 回调函数指针, 用来在坐标(x, y)画一个brick
static void (*draw_box)(uint8_t x, uint8_t y, bool box) = NULL;
// 回调函数指针, 获取一个随机数
static uint8_t (*get_random_num)(void) = NULL;
// 回调函数指针, 返回下一个brick的信息
static void (*return_next_brick_info)(uint16_t info) = NULL;
// 回调函数指针, 当有消行时调用
static void (*return_remove_line_num)(uint8_t line) = NULL;

static bool is_game_over = false;


// 为了preview brick显示美观, 将方块在4 * 4 的点阵中居中
// 实际上这个是可以直接使用方块数据表的, 但要略做更改, 懒得再算表了
static const uint16_t preview_brick_table[BRICK_TYPE] =
{
    0x0360,     // S
    0x0C60,     // Z
    0x0446,     // L
    0x0226,     // J
    0x2222,     // I
    0x0660,     // O
    0x0E40,     // T
};


// 方块数据表
static const uint16_t brick_table[BRICK_TYPE][BRICK_NUM_OF_TYPE] =
{
    { 0x6C00, 0x4620, 0x06C0, 0x8C40 },     // S
    { 0xC600, 0x2640, 0x0C60, 0x4C80 },     // Z
    { 0x88C0, 0xE800, 0x6220, 0x02E0 },     // L
    { 0x2260, 0x08E0, 0xC880, 0xE200 },     // J
    { 0x4444, 0x0F00, 0x2222, 0x00F0 },     // I
    { 0xCC00, 0xCC00, 0xCC00, 0xCC00 },     // O
    { 0xE400, 0x2620, 0x04E0, 0x8C80 }      // T
};


// 旋转掩码表
static const uint16_t rotate_mask[BRICK_TYPE][BRICK_NUM_OF_TYPE] =
{
    { 0xEE20, 0x66E0, 0x8EE0, 0xECC0 },     // S
    { 0xE660, 0x2EE0, 0xEE80, 0xCCE0 },     // Z
    { 0xECC0, 0xEE20, 0x66E0, 0x8EE0 },     // L
    { 0x2EE0, 0xCCE0, 0xEE80, 0xE660 },     // J
    { 0x7FCC, 0xEF33, 0x33FE, 0xCCF0 },     // I
    { 0xCC00, 0xCC00, 0xCC00, 0xCC00 },     // O
    { 0xE620, 0x26E0, 0x8CE0, 0xEC80 }      // T
};

// 下一个方块的y坐标初始值
static const int8_t brick_start_y[BRICK_TYPE] =
{
//   S   Z   L   J   I   O   T
    -2, -2, -3, -3, -4, -2, -2
};

// 地图数组
// map[0]是地图的最上方
static int16_t map[TETRIS_MAP_HEIGHT];
// 地图备份, 保存上一次的数据, 解决屏幕闪烁问题
static int16_t map_backup[TETRIS_MAP_HEIGHT];

static brick_t curr_brick;              // 当前方块
static brick_t next_brick;              // 下一个方块

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \brief  创建一个新的方块
 *
 * \return
 */
static brick_t create_new_brick(void)
{
    brick_t brick;
    uint8_t bt = get_random_num() % BRICK_TYPE;

    // 初始坐标
    brick.x = BRICK_START_X;
    brick.y = brick_start_y[bt];

    // 记录种类
    brick.index = bt << 4;
    brick.brick = brick_table[bt][0];

    return brick;
}


/**
 * \brief  将地图数组中的内容同步到屏幕, 只同步改变的部分
 */
void tetris_sync(void)
{
    uint8_t x, y;

    // 为了解决全图更新时屏幕闪烁的问题
    // 新增一个备份区, 每次只更新不一样的部分
    for (y = 0; y < TETRIS_MAP_HEIGHT; y++)
    {
        // 只更新不一样的部分
        if (map[y] != map_backup[y])
        {
            for (x = 0; x < TETRIS_MAP_WIDTH; x++)
            {
                if (GET_BIT(map[y], x) != GET_BIT(map_backup[y], x))
                    draw_box(x, y, GET_BIT(map[y], x));
            }
        }
    }

    for (y = 0; y < TETRIS_MAP_HEIGHT; y++)
        map_backup[y] = map[y];

    return;
}



/**
 * \brief  同步所有
 */
void tetris_sync_all(void)
{
    uint8_t x, y;

    for (y = 0; y < TETRIS_MAP_HEIGHT; y++)
    {
        for (x = 0; x < TETRIS_MAP_WIDTH; x++)
        {
            draw_box(x, y, GET_BIT(map[y], x));
        }
    }

    return;
}



/**
 * \brief  game over?
 *
 * \return
 */
bool tetris_is_game_over(void)
{
    return is_game_over;
}

/**
 * \brief  在地图数组中画指定方块
 *
 * \param  brick
 */
static void draw_brick(const brick_t brick)
{
    uint8_t i, j;

    for (i = 0; i < BRICK_HEIGHT; i++)
    {
        for (j = 0; j < BRICK_WIDTH; j++)
        {
            // 保证在地图区域内, 因为SET_BIT()只是一个宏, 对数组边界不会进行检查
            // 所以在这里检查数组(地图)边界是必须的, 但是只要检查上边界就可以
            // 因为如果要调用此函数时已经经过冲突检测, 所以其它条件必然符合
            if (brick.y + i >= 0
                // && brick.y < TETRIS_MAP_HEIGHT
                && GET_BIT(brick.brick, 15 - (i * BRICK_WIDTH + j)))
            {
                SET_BIT(map[i + brick.y], j + brick.x);
            }
        }
    }

    return;
}




/**
 * \brief  在方块数组中清除指定方块
 *
 * \param  brick
 */
static void clear_brick(const brick_t brick)
{
    uint8_t i, j;

    for (i = 0; i < BRICK_HEIGHT; i++)
    {
        for (j = 0; j < BRICK_WIDTH; j++)
        {
            // 保证在地图区域内
            if (brick.y + i >= 0
                // && brick.y < TETRIS_MAP_HEIGHT
                && GET_BIT(brick.brick, 15 - (i * BRICK_WIDTH + j)))
            {
                CLR_BIT(map[i + brick.y], j + brick.x);
            }
        }
    }

    return;
}


/**
 * \brief  冲突检测, 检测之前要将当前方块从地图数组中清掉.
 *
 * \param  dest 目标位
 *
 * \retval true 方块在目标位有冲突
 *         false 方块在目标位无冲突
 */
static bool is_conflict(const brick_t dest)
{
    int8_t column, row;
    bool exp = true;

    for (column = 0; column < BRICK_HEIGHT; column++)
    {
        for (row = 0; row < BRICK_WIDTH; row++)
        {
            // 依次检测每一个box
            if ((GET_BIT(dest.brick, (15 - (column * BRICK_WIDTH + row)))))
            {
                // box在地图外的情况(只存在新方块刚被创建时)
                // 这时不用检测地图部分(因为没在地图内)
                // 只需要检查左右边界就可以了, 下边界也没必要检查, 因为肯定不会越界
                if ((dest.y + column) < 0)
                {
                    exp = (((row + dest.x) > (TETRIS_MAP_WIDTH - 1))        // 右边界
                        || ((row + dest.x) < 0));                           // 左边界
                        // || ((column + dest.y) > (TETRIS_MAP_HEIGHT - 1)));  // 下边界
                }
                else
                {
                    // 此时box在地图内, 检查下边界, 还要检测box在地图内是否有冲突
                    exp = (((row + dest.x) > (TETRIS_MAP_WIDTH - 1))        // 右边界
                        || ((row + dest.x) < 0)                             // 左边界
                        || ((column + dest.y) > (TETRIS_MAP_HEIGHT - 1))    // 下边界
                        || (GET_BIT(map[column + dest.y], (row + dest.x))));// 地图内
                }
                if (exp)
                    return true;
            }
        }
    }

    return false;
}


/**
 * \brief
 *
 * \param  draw_box_to_map
 * \param  get_random
 * \param  next_brick_info
 * \param  remove_line_num
 */
void tetris_init(void (*draw_box_to_map)(uint8_t x, uint8_t y, bool box),
                 uint8_t (*get_random)(void),
                 void (*next_brick_info)(uint16_t info),
                 void (*remove_line_num)(uint8_t line))
{
    uint8_t i;

    draw_box = draw_box_to_map;
    get_random_num = get_random;
    return_next_brick_info = next_brick_info;
    return_remove_line_num = remove_line_num;
    is_game_over = false;

    // 初始化地图
    for (i = 0; i < TETRIS_MAP_HEIGHT; i++)
        map[i] = 0;

    curr_brick = create_new_brick();
    next_brick = create_new_brick();

    // 返回预览方块信息
    if (return_next_brick_info != NULL)
        return_next_brick_info(preview_brick_table[next_brick.index >> 4]);

    draw_brick(curr_brick);
    tetris_sync_all();

    return;
}

/**
 * \brief  消行
 */
static void line_clear_check(void)
{
    uint8_t i, j, l;

    l = 0;

    // FIXME
    // 消行, map[0]实际上是地图的顶端
    // 从顶端开始向下扫, 每遇到一行满的
    // 就以此开始替换
    for (i = 0; i < TETRIS_MAP_HEIGHT; i++)
    {
        if (map[i] >= 0x3FF)
        {
            l++;
            for (j = i; j > 0; j--)
            {
                map[j] = map[j - 1];
            }
            map[0] = 0;
        }
    }

    // 有消行, 返回消行数
    if (return_remove_line_num != NULL)
        return_remove_line_num(l);

    return;
}



/**
 * \brief  移动方块
 *
 * \param  direction
 *
 * \retval true 移动失败
 *         false 移动成功
 */
bool tetris_move(dire_t direction)
{
    brick_t dest_brick = curr_brick;
    bool is_move = false;

    switch ((uint8_t)direction)
    {
        case dire_left:
            dest_brick.x--;
            break;
        case dire_right:
            dest_brick.x++;
            break;
        case dire_down:
            dest_brick.y++;
            break;
        case dire_rotate:
        {
            // 低8位记录变形
            uint8_t i = dest_brick.index & 0x0F;
            i++;
            dest_brick.index = (dest_brick.index & 0xF0) | (i % 4);
            dest_brick.brick = rotate_mask[dest_brick.index >> 4][dest_brick.index & 0x0F];

            break;
        }
        default:
            break;
    }

    // 在检测之前先将当前方块从地图中清掉
    clear_brick(curr_brick);

    // 无冲突, 更改之
    if (!is_conflict(dest_brick))
    {
        // 旋转, 要方块信息从旋转mask改回来
        if (direction == dire_rotate)
        {
            dest_brick.brick = brick_table[dest_brick.index >> 4][dest_brick.index & 0x0F];
        }
        curr_brick = dest_brick;
        is_move = true;
    }
    else
    {
        // 不可移动, 且向下不可移动
        if (direction == dire_down)
        {
            // 先将当前方块画到地图中
            draw_brick(curr_brick);
            // 如果下落完成时当前方块还有部分在地图外
            // 或者下一个方块无法再放进地图, 游戏结束
            if (curr_brick.y + 1 <= 0)
            {
                is_game_over = true;
            }
            // 消行
            line_clear_check();
            // 产生新方块
            curr_brick = next_brick;
            next_brick = create_new_brick();
            // 预览方块信息
            if (return_next_brick_info != NULL)
                return_next_brick_info(preview_brick_table[next_brick.index >> 4]);
        }
        is_move = false;
    }

    draw_brick(curr_brick);

    return is_move;
}


/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/


