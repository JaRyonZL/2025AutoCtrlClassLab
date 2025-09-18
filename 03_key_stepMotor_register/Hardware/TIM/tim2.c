/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-11 10:26:38
 */
// FileName: tim2.c 

#include "includes.h"

/**
 * @brief TIM2_Init 用于输出PWM信号，提供给SG90舵机
 * @param void 
 * @return void
 */
void TIM2_Init(void) 
{
    /* 1. 开启时钟 PA0 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    /* 2. 配置GPIO工作模式: 复用推挽输出 50MHz */
    GPIOA->CRL |=  GPIO_CRL_MODE0;
    GPIOA->CRL |= GPIO_CRL_CNF0_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF0_0;

    /* 3. 定时器配置 20ms */
    // 3.1 设置预分频值
    TIM2->PSC = TIM2_PSC; // 72000000 / 720 = 100000 10^5Hz 1/100000s=1/100ms 

    // 3.2 设置重装载值
    TIM2->ARR = TIM2_ARR;  // 20ms / 0.01 = 2000 - 1
    TIM2->CR1 |= TIM_CR1_ARPE;  // 使能自动重装载预装载

    // 3.3 配置计数方向: 向上计数 0,1...
    TIM2->CR1 &= ~TIM_CR1_DIR;

    // 3.4 设置比较值 0.5~2.5ms中值
    TIM2->CCR1 = 150;

    // 3.5 配置pwm模式 
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;     // 输出模式
    TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);   // PWM模式1
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;     // 产生更新事件时写入

    // 3.6 配置输出极性和使能
    TIM2->CCER &= ~TIM_CCER_CC1P;
    TIM2->CCER |= TIM_CCER_CC1E;

}

/**
 * @brief TIM2_Start 开启定时器
 * @param void 
 * @return void
 */
void TIM2_Start(void) 
{
    TIM2->EGR |= TIM_EGR_UG;     // 产生更新事件
    TIM2->CR1 |= TIM_CR1_CEN;    // 使能定时器
}

/**
 * @brief TIM2_Close 关闭定时器
 * @param void 
 * @return void
 */
void TIM2_Close(void) 
{
    TIM2->CR1 &= ~TIM_CR1_CEN;    // 使能定时器
    TIM2->EGR &= ~TIM_EGR_UG;     // 产生更新事件  
}

/**
 * @brief TIM2_SetCompare 设置pwm占空比
 * @param  uint16_t compare
 * @return void
 */
void TIM2_SetCompare(uint16_t compare) 
{
    TIM2->CCR1 = compare;
}
