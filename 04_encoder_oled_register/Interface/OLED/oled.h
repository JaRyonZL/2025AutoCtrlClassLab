#ifndef __OLED_H
#define __OLED_H

#include "includes.h"

// 定义显存缓冲区
extern uint8_t dispMemBuf[8][128];

// OLED初始化
void OLED_Init(void);

int OLED_Pow(int x, int y);

// 写入一字节命令
void OLED_WriteCommand(uint8_t command);

// 写入一字节数据
void OLED_WriteData(uint8_t * data, uint8_t len);

// 设置光标显示初始位置
void OLED_SetCursorPos(uint8_t x, uint8_t page);

// 更新显存数据
void OLED_Update(void);

// 清屏
void OLED_ClearAll(void);

// 局部清屏
void OLED_ClearArea(uint8_t col, uint8_t row, uint8_t width, uint8_t height);

// 显示单个字符
void OLED_ShowChar(uint8_t col, uint8_t row, char ch, uint8_t fontSize);

// 显示字符串
void OLED_ShowString(uint8_t col, uint8_t row, char * string, uint8_t fontSize);

// 显示十进制数字
void OLED_ShowDecNum(uint8_t col, uint8_t row, int number, uint8_t length, uint8_t fontSize);

// 显示十进制小数
void OLED_ShowDecNum1(uint8_t col, uint8_t row, int number, uint8_t length, uint8_t fontSize);
void OLED_ShowFloat(uint8_t col, uint8_t row, double number, uint8_t intLen, uint8_t fraLen, uint8_t fontSize);

// 显示图像
void OLED_ShowImage(uint8_t col, uint8_t row, uint8_t height, uint8_t width, const uint8_t * image);

// 显示汉字
void OLED_ShowChinese(uint8_t col, uint8_t row, char * chinese);

// 绘制点
void OLED_DrawPoint(uint8_t X, uint8_t Y);

#endif
