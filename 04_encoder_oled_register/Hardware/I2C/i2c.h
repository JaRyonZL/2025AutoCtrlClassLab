/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-21 11:49:57
 */
// FileName: i2c.h

#ifndef __I2C_H
#define __I2C_H

#include "includes.h"

// 宏定义
#define ACK 0
#define NACK 1

// SCL、SDA线拉低拉高
#define SCL_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR0)
#define SCL_HIGH (GPIOA->ODR |= GPIO_ODR_ODR0)

#define SDA_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR1)
#define SDA_HIGH (GPIOA->ODR |= GPIO_ODR_ODR1)

// 主机读取从机信号
#define READ_SDA (GPIOA->IDR & GPIO_IDR_IDR1)

// I2C通讯基本延时
#define I2C_DELAY (Delay_us(1))

// 初始化
void I2C_Init(void);

// 主设备发出起始信号
void I2C_Start(void);

// 主设备发出停止信号
void I2C_Stop(void);

// 主设备发出应答响应
void I2C_Ack(void);

// 主设备发出非应答响应
void I2C_Nack(void);

// 主机等待从机发出响应
uint8_t I2C_Wait4Ack(void);

// 主机向从机写入一个字节的数据（发送）
void I2C_SendByte(uint8_t byte);

// 主机向从机读取一个字节的数据（接收）
uint8_t I2C_ReadByte(void);

#endif
