#ifndef __OLEDDATA_H
#define __OLEDDATA_H

#include "includes.h"

// 8x16像素数据
extern const uint8_t OLED_Font8x16[][16];
// 6x8像素数据
extern const uint8_t OLED_Font6x8[][6];
// 显示图形
extern const uint8_t OLED_Image[][16];
// 汉字结构体
typedef struct
{
	// 汉字字符串
	char index[4];
	// 汉字对应编码（UTF-8）
	uint8_t data[32];
	
}SingleChineseFont_t;

// 汉字字符集
extern const SingleChineseFont_t OLED_ChineseFont16x16[];

#endif
