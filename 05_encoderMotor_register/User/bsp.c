// FileName: bsp.c

#include "includes.h"

void BSPInit(void)
{
    TIM3_Init(); // 定时器初始化
    TIM2_Init();
    USART_Init();     // 串口初始化
    ENCODER_Init();   // 编码器初始化
    JGA25_370_Init(); // 电机初始化
}
