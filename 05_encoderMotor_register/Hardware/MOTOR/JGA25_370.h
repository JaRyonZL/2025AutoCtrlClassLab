/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-25 12:07:01
 */
// FileName: JGA25_370.h

#ifndef __JAG25_370_H
#define __JAG25_370_H

#include "stm32f10x.h"

// 设置电机速度和方向
#define PWM_MAX 1000          // TIM2 最大 CCR
#define COUNT_PER_REV 937     // 电机轴一圈对应编码器脉冲

void JGA25_370_Init(void);
void JGA25_370_SetSpeed(int32_t speed);
void JGA25_370_Stop(void);
void JGA25_370_Brake(void);


#endif

