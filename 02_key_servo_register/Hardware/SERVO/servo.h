/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-11 10:28:58
 */


#ifndef __SERVO_H
#define __SERVO_H

// define ---------------------------------------------------
#define MIN_SERVO_ANGLE 1
#define MAX_SERVO_ANGLE 180
#define MID_SERVO_ANGLE 90

// function -------------------------------------------------
void SERVO_Init(void);
static uint16_t SERVO_Angle2Compare(uint8_t angle);
void SERVO_SetAngle(uint8_t angle);
uint8_t SERVO_GetServoAngle(void);
void SERVO_NextAngle(void);


#endif // !__SERVO_H
