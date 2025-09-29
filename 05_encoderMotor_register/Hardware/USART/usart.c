#include "includes.h"

// 初始化
void USART_Init(void)
{
    // 1. 开启GPIO时钟 PA9 PA10
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 2. 设置GPIO工作模式
    // PA9 TX 输出，复用推挽输出 MODE-11 CNF-10
    // PA10 RX 输入，浮空输入 MODE-00 CNF-01
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;

    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3. 串口配置
    // 3.1 设置波特率
    USART1->BRR = 0x271;         // 115.2 Kpbs

    // 3.2 开启模块及收发使能
    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_RE;

    // 3.3 其他配置（字长、奇偶校验、停止位）
    USART1->CR1 &= ~USART_CR1_M;
    USART1->CR1 &= ~USART_CR1_PCE;
    USART1->CR2 &= ~USART_CR2_STOP;
}

// 发送一个字符
void USART_SendChar(uint8_t ch)
{
    // 当发送的数据不为空时等待，TXE为1则可以继续写入数据
    while ((USART1->SR & USART_SR_TXE) == 0)
    {}

    // 发送一个字符
    USART1->DR = ch;    
}

// 重定向fputc函数
int fputc(int ch, FILE * file)
{
    USART_SendChar((uint8_t)ch);
    return (int)ch;
}
