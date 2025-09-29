/*
 * @Descripttion: TIM2配置生成PWM信号驱动电机
 * @Author: JaRyon
 * @version:
 * @Date: 2025-09-25 12:03:52
 */
// FileName: tim2.c

#include "includes.h"

// TIM2 PWM初始化 (PA3 - PWMA)
void TIM2_Init(void)
{
    /* 1. 开启GPIO与TIM2时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. 配置工作模式 PA3 复用推挽50MHz */
    GPIOA->CRL |= GPIO_CRL_CNF3_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF3_0;
    GPIOA->CRL |= GPIO_CRL_MODE3;

    /* 3. 配置定时器TIM2*/
    // 3.1 设置预分频值、重装载值 72分频、10^3后更新 即频率1000，周期1ms(PWM)
    TIM2->PSC = 72 - 1;
    TIM2->ARR = 1000 - 1;

    // 3.2 设置模式1、通道4
    TIM2->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
    TIM2->CCMR2 &= ~TIM_CCMR2_OC4M_0;
    TIM2->CCER |= TIM_CCER_CC4E; 
    // 3.3 使能预装载
    TIM2->CCMR2 |= TIM_CCMR2_OC4PE; 
    TIM2->CR1 |= TIM_CR1_ARPE;      // ARR预装载
    // 3.3 使能定时器2
    TIM2->CR1 |= TIM_CR1_CEN;
}

// 设置PWM占空比
void TIM2_SetCompare(uint16_t compare)
{
    if (compare > 1000)
        compare = 1000;
    TIM2->CCR4 = compare;
}
