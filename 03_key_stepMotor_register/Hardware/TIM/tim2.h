/*
 * @Descripttion: tim2 pwm signal 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-11 10:26:49
 */

#ifndef __TIM_H
#define __TIM_H

// define ---------------------------------------
#define TIM2_PSC    719
#define TIM2_ARR    1999
#define MIN_TIM2_COMPARE 50
#define MAX_TIM2_COMPARE 250

void TIM2_Init(void);
void TIM2_Start(void);
void TIM2_Close(void);
void TIM2_SetCompare(uint16_t compare);


#endif // !__TIM_H
