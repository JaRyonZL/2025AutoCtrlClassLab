/*
 * @Descripttion:
 * @Author: JaRyon
 * @version:
 * @Date: 2025-09-18 14:03:30
 */
// FileName: stepMotor.c

#include "includes.h"

uint8_t g_motorDir = 1; // 电机方向，1正转，0反转

// 四相八拍通电顺序表
uint8_t stepSequence[8][4] =
    {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1}};

// 步进电机初始化
void STEPMOTOR_Init(void)
{
    // 使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 配置为推挽输出模式（50MHz）mode-11 cnf-00
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3);
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3);

    // 初始状态全部置低
    GPIOA->ODR &= ~(GPIO_ODR_ODR0 | GPIO_ODR_ODR1 | GPIO_ODR_ODR2 | GPIO_ODR_ODR3);
}

// 步进电机运行函数
// dir: 1=正转, 0=反转; angle: 转动角度
void STEPMOTOR_Run(uint8_t dir, float angle)
{
    // 28BYJ-48实际每圈需要4096步（8拍×512减速比）
    // 90度 = 4096 / 4 = 1024步
    uint16_t totalSteps = (uint16_t)(angle * 4096 / 360);

    uint8_t stepIndex = 0;

    for (uint16_t i = 0; i < totalSteps; i++)
    {
        // 清除当前输出
        GPIOA->ODR &= ~(GPIO_ODR_ODR0 | GPIO_ODR_ODR1 | GPIO_ODR_ODR2 | GPIO_ODR_ODR3);

        // 设置当前步的电平
        if (stepSequence[stepIndex][0])
            GPIOA->ODR |= GPIO_ODR_ODR0;
        if (stepSequence[stepIndex][1])
            GPIOA->ODR |= GPIO_ODR_ODR1;
        if (stepSequence[stepIndex][2])
            GPIOA->ODR |= GPIO_ODR_ODR2;
        if (stepSequence[stepIndex][3])
            GPIOA->ODR |= GPIO_ODR_ODR3;

        // 控制方向
        if (dir)
        {
            stepIndex = (stepIndex + 1) % 8; // 正转
        }
        else
        {
            stepIndex = (stepIndex + 7) % 8; // 反转（避免负数）
        }

        Delay_ms(2); // 控制转速，可根据需要调整
    }

    // 停止后全部置低
    GPIOA->ODR &= ~(GPIO_ODR_ODR0 | GPIO_ODR_ODR1 | GPIO_ODR_ODR2 | GPIO_ODR_ODR3);
}
