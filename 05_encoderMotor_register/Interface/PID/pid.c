/*
 * @Descripttion:
 * @Author: JaRyon
 * @version:
 * @Date: 2025-09-25 12:21:02
 */
// FileName: pid.c

#include "includes.h"

// PID控制器初始化
// PID结构体指针 kp/ki/kd
void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->SetSpeed = 0;
    pid->ActualSpeed = 0;
    pid->Err = 0;
    pid->LastErr = 0;
    pid->Integral = 0;
}

// PID计算
// pid结构体指针 实际速度(转速)
float PID_Calc(PID_TypeDef *pid, float speed)
{
    pid->ActualSpeed = speed;
    pid->Err = pid->SetSpeed - pid->ActualSpeed;
    pid->Integral += pid->Err;

    float output = pid->Kp * pid->Err + pid->Ki * pid->Integral + pid->Kd * (pid->Err - pid->LastErr);

    // 限制 PWM 范围
    if (output > PWM_MAX)
        output = PWM_MAX;
    if (output < -PWM_MAX)
        output = -PWM_MAX;

    pid->LastErr = pid->Err;
    return output;
}
