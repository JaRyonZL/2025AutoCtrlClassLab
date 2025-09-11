/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-11 10:28:49
 */
// FileName: servo.c 

#include "includes.h"

// 当前舵机角度 默认中值 （内部使用）
static uint8_t current_angle = MID_SERVO_ANGLE;
// 当前角度状态
static uint8_t angle_state = 2;  // 1-0° 2-90° 3-180°（初始90°）


/**
 * @brief SERVO_Init
 * @param void 
 * @return void
 */
void SERVO_Init(void) 
{
    TIM2_Init();
    TIM2_Start();
    SERVO_SetAngle(MID_SERVO_ANGLE);
}

/**
 * @brief SERVO_Angle2Compare 角度转换为占空比
 * @param uint8_t angle 
 * @return static uint16_t
 * @attention 内部调用
 */
static uint16_t SERVO_Angle2Compare(uint8_t angle) 
{
    // 角度限幅
    if (angle > MAX_SERVO_ANGLE)    {angle = MAX_SERVO_ANGLE;}
    if (angle < MIN_SERVO_ANGLE)    {angle = MIN_SERVO_ANGLE;}
    
    // 90 -> 150 180 -> 250 线性映射y=lx+b 
    // 0 + delta cycle / delta angle * angle
    return (uint16_t)(MIN_TIM2_COMPARE + (uint16_t)(MAX_TIM2_COMPARE - MIN_TIM2_COMPARE) / (uint8_t)(MAX_SERVO_ANGLE - MIN_SERVO_ANGLE) * angle);
}

/**
 * @brief SERVO_SetAngle 设置舵机角度
 * @param  uint8_t angle
 * @return void
 */
void SERVO_SetAngle(uint8_t angle) 
{
    uint16_t compare;

    // 角度限幅
    if (angle > MAX_SERVO_ANGLE)    {angle = MAX_SERVO_ANGLE;}
    if (angle < MIN_SERVO_ANGLE)    {angle = MIN_SERVO_ANGLE;}
    
    /* 角度更新与转换 */
    current_angle = angle;
    compare = SERVO_Angle2Compare(angle);
    TIM2_SetCompare(compare);
}

/**
 * @brief SERVO_GetServoAngle 获取当前舵机角度（0~180）
 * @param void 
 * @return uint8_t
 */
uint8_t SERVO_GetServoAngle(void) 
{
    return current_angle;
}


/**
 * @brief SERVO_NextAngle 切换当前舵机角度（0~180）
 * @param void 
 * @return void
 */
void SERVO_NextAngle(void)
{
    // 循环切换角度状态
    angle_state++;
    if (angle_state > 4)
    {
        angle_state = 1;
    }
    
    // 根据状态设置对应角度
    switch (angle_state)
    {
        case 1:
            SERVO_SetAngle(ANGLE_0);
            break;
        case 2:
            SERVO_SetAngle(ANGLE_90);
            break;
        case 3:
            SERVO_SetAngle(ANGLE_180);
            break;
        case 4:
            SERVO_SetAngle(ANGLE_90);
            break;
    }
}
