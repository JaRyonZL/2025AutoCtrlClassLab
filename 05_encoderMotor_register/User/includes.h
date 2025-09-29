/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-25 12:03:52
 */
// FileName: includes.h 

#ifndef __INCLUDES_H
#define __INCLUDES_H

#include "stm32f10x.h"
#include <stdio.h>
#include <math.h>
#include "string.h"
#include "Delay.h"
// ---------------- 硬件外设 -----------------
#include "bsp.h"
#include "tim2.h"
#include "tim3.h"
#include "usart.h"
#include "encoder.h"
// ---------------- 电机驱动 ------------------
#include "JGA25_370.h"
// ---------------- PID控制 -------------------
#include "pid.h"

#endif // !__INCLUDES_H
