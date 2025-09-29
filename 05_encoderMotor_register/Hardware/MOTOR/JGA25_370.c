/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-25 12:05:48
 */
#include "includes.h"

// 电机初始化 (PB1=AIN1, PB0=AIN2，处理电机转动方向)
void JGA25_370_Init(void)
{
    /* 1. 开启GPIOB时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2. 配置GPIO工作模式 PB1 PB0 通用推挽输出 50MHz */
    GPIOB->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOB->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1; 

    /* 3. 初始化电机状态: 停止*/
    JGA25_370_Stop();
}

// 设置电机速度，speed范围 -1000~1000，对应 PWM 0~1000
// speed = 1000 → 电机一圈编码器 Count ≈ 937
void JGA25_370_SetSpeed(int32_t speed)
{
    // 限制范围
    if (speed > 1000) speed = 1000;
    if (speed < -1000) speed = -1000;

    uint16_t pwm_val = (uint16_t)((fabs(speed) * PWM_MAX) / COUNT_PER_REV); // 线性映射

    // 设置方向，AIN1 2组合
    if(speed >= 0) // 正传
    {
        GPIOB->BSRR = GPIO_BSRR_BS1; // PB1高
        GPIOB->BRR  = GPIO_BRR_BR0;   // PB0低
    }
    else // 反转
    {
        GPIOB->BRR  = GPIO_BRR_BR1;   // PB1低
        GPIOB->BSRR = GPIO_BSRR_BS0;  // PB0高
    }

    TIM2_SetCompare(pwm_val);   // 设置 PWM
}


// 电机停止
void JGA25_370_Stop(void)
{
    // 减速滑行，PWM置0
    GPIOB->BRR = GPIO_BRR_BR0 | GPIO_BRR_BR1; 
    TIM2_SetCompare(0);
}

// 电机刹车
void JGA25_370_Brake(void)
{
    GPIOB->BSRR = GPIO_BSRR_BS0 | GPIO_BSRR_BS1; // PB0和PB1都高电平
    TIM2_SetCompare(1000);
}
