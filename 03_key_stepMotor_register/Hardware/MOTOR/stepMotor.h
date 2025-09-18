/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-18 14:03:46
 */
// FileName:stepMotor.h

#ifndef __STEPMOTOR_H
#define __STEPMOTOR_H

#include "includes.h"

extern uint8_t g_motorDir;


void STEPMOTOR_Init(void);
void STEPMOTOR_Run(uint8_t dir, float angle);

#endif
