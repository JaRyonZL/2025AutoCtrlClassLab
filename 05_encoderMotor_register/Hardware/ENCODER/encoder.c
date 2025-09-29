#include "includes.h"

// 上次计数
static int32_t last_cnt = 0;
static int32_t total_cnt = 0;

void ENCODER_Init(void)
{
    /* 1. 开启 GPIOB 和 TIM4 时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    /* 2. 配置 PB6/PB7 为浮空输入（TIM 编码器模式用输入） */
    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
    GPIOB->CRL |= (GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);

    /* 3. 配置定时器TIM4 */
    // 3.1 先关闭计数器
    TIM4->CR1 &= ~TIM_CR1_CEN;
    // 3.2 设置编码器模式3：上下降沿均进行输入捕获
    TIM4->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
    TIM4->SMCR &= ~TIM_SMCR_SMS_2;
    // 3.3 配置模式为输入 映射到TI1/TI2
    TIM4->CCMR1 |= (TIM_CCMR1_CC1S_0) | (TIM_CCMR1_CC2S_0);
    TIM4->CCMR1 &= ~(TIM_CCMR1_CC1S_1) | (TIM_CCMR1_CC2S_1);
    // 3.4 设置捕获极性 实际情况下该设置不影响实际捕获极性
    TIM4->CCER &= ~((TIM_CCER_CC1P) | (TIM_CCER_CC2P));
    // 3.5 设置自动重装载寄存器最大
    TIM4->ARR = TIM_MAX;
    // 3.6 清零计数器
    TIM4->CNT = 0;
    last_cnt = 0;
    total_cnt = 0;
    // 3.7 使能捕获、计数器
    TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM4->CR1 |= TIM_CR1_CEN;
}

// 获取累计脉冲
int32_t ENCODER_GetCount(void)
{
    int32_t cnt_now = (int32_t)TIM4->CNT; // 当前脉冲数
    int32_t delta = cnt_now - last_cnt;   // 一次累计计数

    // 处理计数器溢出
    if (delta > 32767)
        delta -= 65536;
    else if (delta < -32768)
        delta += 65536;

    total_cnt += delta; // 总脉冲数
    last_cnt = cnt_now; // 上一次脉冲数

    return total_cnt;
}

// 获取编码器实际转速(r/min)
// 中断调用 50ms
// cnt_now 当前累计计数
float ENCODER_GetSpeed_rpm(volatile int32_t * cnt_now)
{
    static int32_t prev_cnt = 0;
    *cnt_now = ENCODER_GetCount();
    int32_t delta = *cnt_now - prev_cnt;
    prev_cnt = *cnt_now;

    float pulses_per_sec = delta / DELTA_T;
    return pulses_per_sec / PULSES_PER_REV * 60.0f;
}
