/*
 * @Descripttion:
 * @Author: JaRyon
 * @version:
 * @Date: 2025-09-25 12:23:45
 */
// FileName: tim3.c

#include "includes.h"

volatile float rpm_global = 0;     // 中断计算的平滑 rpm
volatile int32_t count_global = 0;

// 初始化 TIM3，用于定时中断
void TIM3_Init(void)
{
    /* 1. 开启TIM3时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;


    /* 2. 配置TIM3时钟 */
    // 2.1 设置预分频系数与重装载值
    TIM3->PSC = 7200 - 1;  // 72MHz / 7200 = 10kHz
    TIM3->ARR = 500 - 1;   // 10kHz / 500 = 20Hz →50ms
    // 2.2 清零计数器
    TIM3->CNT = 0;
    // 2.3 使能TIM3更新中断、使能计数器
    TIM3->DIER |= TIM_DIER_UIE; // 溢出时产生中断
    TIM3->CR1 |= TIM_CR1_CEN;  

    NVIC_EnableIRQ(TIM3_IRQn);
}

// TIM3 中断处理函数，用于周期计算 rpm
void TIM3_IRQHandler(void)
{
    if(TIM3->SR & TIM_SR_UIF) // 更新中断标志
    {
        TIM3->SR &= ~TIM_SR_UIF; // 清除标志

        rpm_global = ENCODER_GetSpeed_rpm(&count_global);
    }
}
