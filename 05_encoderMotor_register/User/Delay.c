#include "includes.h"

// 延时函数 微秒为单位 利用系统的嘀嗒定时器 系统CPU频率 72MHz 一次嘀嗒 1/72 us
void Delay_us(uint16_t us)
{
	// 1. 装载一个计数值 1us 嘀嗒72次
	SysTick->LOAD = 72 * us;

	// 2. 配置嘀嗒定时器 使用系统时钟（1）、不开启中断（0）、打开使能寄存器（1）
	SysTick->CTRL |= 0x05;

	// 3. 等待计数值变为0，即判断配置标志位COUNTFLAG是否变为1 （我的理解：每一次计数完毕后会置0）
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0)
	{}

	// 4. 关闭定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE;   //（ 我的理解：使能寄存器置0）
}

void Delay_ms(uint16_t ms)
{
	while(ms--)
	{
		Delay_us(1000);
	}
}

void Delay_s(uint16_t s)
{
	while(s--)
	{
		Delay_ms(1000);
	}
}

