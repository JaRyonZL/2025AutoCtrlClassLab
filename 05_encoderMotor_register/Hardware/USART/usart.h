#ifndef __USART_H
#define __USART_H

#include "includes.h"

// 初始化
void USART_Init(void);

// 发送一个字符
void USART_SendChar(uint8_t ch);

#endif
