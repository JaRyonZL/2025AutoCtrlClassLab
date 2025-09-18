/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-18 14:02:04
 */
// FileName: main.c

#include "includes.h"

int main(void)
{
    // 初始化步进电机
	KEY_Init();
    STEPMOTOR_Init();

    while (1)
    {
        // 主循环无需主动扫描按键，中断触发后控制电机
    }
}







