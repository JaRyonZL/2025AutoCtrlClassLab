// FileName: encoder.c

#include "includes.h"

/**
 * @brief ENCODER_Init 旋转编码器初始化
 * @param void
 * @return void
 */
void ENCODER_Init(void)
{
    /* 1. 开启时钟: GPIO TIM3 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. 配置GPIO工作模式: PA6 PA7 上拉输入 */
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
    GPIOA->CRL |= (GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);
    // 上拉
    GPIOA->ODR |= (GPIO_ODR_ODR6 | GPIO_ODR_ODR7);

    /* 3. 设置TIM3预分频与重装载值 */
    TIM3->PSC = 0;
    TIM3->ARR = 0xFFFF;

    /* 4. 配置通道和模式: CH1 CH2输入捕获 四倍频计数 模式3 */
    TIM3->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
    TIM3->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;

    // 使能定时器
    TIM3->CNT = 0;            // 初始计数为0
    TIM3->CR1 |= TIM_CR1_CEN; // 使能定时器
}

/**
 * @brief ENCODER_GetCnt 获取编码器计数值
 * @param void
 * @return uint16_t
 * @attention 返回值为int16_t，正传会+值，反转为-值
 */
int16_t ENCODER_GetCnt(void)
{
    return (int16_t)TIM3->CNT;
}
