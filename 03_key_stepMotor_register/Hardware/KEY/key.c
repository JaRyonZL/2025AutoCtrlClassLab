/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-18 14:02:04
 */
/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-11 10:21:11
 */

// FileName:key.h 

#include "includes.h"

uint8_t count = 0;

/**
 * @brief 按键初始化（PB7，外部中断，下降沿触发）
 * @param void
 * @return void
 */
void KEY_Init(void)
{
    // 1. 开启时钟（GPIOB和AFIO）
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;  // 使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;  // 使能AFIO时钟

    // 2. 配置GPIO工作模式（上拉输入，因为按键另一端接GND）
    GPIOB->CRL &= ~GPIO_CRL_MODE7;       // MODE7=00（输入模式）
    GPIOB->CRL |= GPIO_CRL_CNF7_1;       // CNF7=10（上拉/下拉输入）
    GPIOB->CRL &= ~GPIO_CRL_CNF7_0;
    GPIOB->ODR |= GPIO_ODR_ODR7;         // 上拉（默认高电平）

    // 3. 配置AFIO复用映射（PB7 -> EXTI7）
    AFIO->EXTICR[1] &= ~AFIO_EXTICR2_EXTI7;  // 清除原有映射
    AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI7_PB;  // PB7映射到EXTI7

    // 4. 配置EXTI（下降沿触发，允许中断）
    EXTI->FTSR |= EXTI_FTSR_TR7;         // 允许下降沿触发（按键按下）
    EXTI->RTSR &= ~EXTI_RTSR_TR7;        // 禁止上升沿触发
    EXTI->IMR |= EXTI_IMR_MR7;           // 允许EXTI7中断

    // 5. 配置NVIC（中断优先级和使能）
    NVIC_SetPriorityGrouping(3);         // 全部使用抢占优先级
    NVIC_SetPriority(EXTI9_5_IRQn, 2);   // 设置优先级（值越小优先级越高）
    NVIC_EnableIRQ(EXTI9_5_IRQn);        // 使能EXTI9_5中断
}

/**
 * @brief EXTI9_5中断服务函数（处理PB7中断）
 */
void EXTI9_5_IRQHandler(void)
{
    // 检查是否是PB7触发的中断
    if (EXTI->PR & EXTI_PR_PR7)
    {
        // 清除中断标志位（必须手动清除）
        EXTI->PR |= EXTI_PR_PR7;

        // 软件消抖
        Delay_ms(10);

        // 确认按键按下（PB7接GND，按下时为低电平）
        if (!(GPIOB->IDR & GPIO_IDR_IDR7))
        {
            // 步进电机控制
            // 转动90度
            STEPMOTOR_Run(g_motorDir, 90);
            count++;
            if(count > 4)
            {
                // 切换方向
            g_motorDir = !g_motorDir;
            count=0;
            }
            
        }
    }
}
 
