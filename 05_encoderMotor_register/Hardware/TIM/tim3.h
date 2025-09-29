/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-25 12:23:45
 */
// FileName: tim3.h

#ifndef __TIM3_H
#define __TIM3_H

#include "stm32f10x.h"

extern volatile float rpm_global;     // 中断计算的平滑 rpm
extern volatile int32_t count_global;

void TIM3_Init(void);



#endif // !__TIM_H
