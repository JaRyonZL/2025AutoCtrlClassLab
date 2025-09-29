#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f10x.h"

// ------------------------------------- 宏定义 -----------------------------------------------
#define GEAR_RATIO 21.3f                      // 电机减速比（实际硬件参数）
#define PULSES_PER_REV (PPR * 4 * GEAR_RATIO) // 实际每圈脉冲数
#define PPR 11                                // 基础脉冲数（实际硬件参数）
#define TIM_MAX 0xFFFF                        // 定义定时器最大值
#define DELTA_T 0.05f                         // 定时器更新周期，单位 s（例如 50ms）

// ------------------------------------- 函数声明 ---------------------------------------------
void ENCODER_Init(void);
int32_t ENCODER_GetCount(void);
float ENCODER_GetSpeed_rpm(volatile int32_t * cnt_now);

#endif
