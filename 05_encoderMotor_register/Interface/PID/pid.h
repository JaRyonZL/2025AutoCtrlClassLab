/*
 * @Descripttion:
 * @Author: JaRyon
 * @version:
 * @Date: 2025-09-25 12:21:10
 */
// FileName: pid.h

#ifndef __PID_H__
#define __PID_H__

#include "stm32f10x.h"

// PID控制结构体
typedef struct
{
    float Kp, Ki, Kd;
    float SetSpeed;    // 目标速度
    float ActualSpeed; // 实际速度
    float Err, LastErr;
    float Integral;
} PID_TypeDef;

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd);
float PID_Calc(PID_TypeDef *pid, float speed);

#endif
