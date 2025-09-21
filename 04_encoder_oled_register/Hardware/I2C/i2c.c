// FileName: i2c.c

#include "includes.h"

// 初始化
void I2C_Init(void)
{
    // SCL-PA0  SDA-PA1

    // 1. 配置时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. 设置GPIO工作模式 通用开漏输出 cnf-01 mode-11
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOA->CRL &= ~(GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1);
    GPIOA->CRL |= (GPIO_CRL_CNF0_0 | GPIO_CRL_CNF1_0);

}

// 主设备发出起始信号
void I2C_Start(void)
{
    // 1. SCL、SDA拉高
    SDA_HIGH;
    SCL_HIGH;
    I2C_DELAY;

    // 2. SCL保持不变、SDA拉低，发出起始信号
    SDA_LOW;
    I2C_DELAY;
}

// 主设备发出停止信号
void I2C_Stop(void)
{
    // 1. SCL拉高、SDA拉低
    SDA_LOW;
    SCL_HIGH;
    I2C_DELAY;

    // 2. SCL保持不变、SDA拉高
    SDA_HIGH;
    I2C_DELAY;
}

// 主设备发出应答响应
void I2C_Ack(void)
{
    // 1. SDA拉高、SCL拉低
    SDA_HIGH;
    SCL_LOW;
    I2C_DELAY;

    // 2. SCL保持不变、SDA拉低，主机发出应答
    SDA_LOW;
    I2C_DELAY;

    // 3. SCL拉高、SDA保持不变，开始信号采样
    SCL_HIGH;
    I2C_DELAY;

    // 4. SCL拉低、SDA保持不变，结束信号采样
    SCL_LOW;
    I2C_DELAY;

    // 5. SDA拉高，释放数据总线
    SDA_HIGH;
    I2C_DELAY;
}

// 主设备发出非应答响应
void I2C_Nack(void)
{
    // 1. SDA拉高、SCL拉低
    SDA_HIGH;
    SCL_LOW;
    I2C_DELAY;

    // 2. SDA保持不变、SCL拉高，开始非应答信号采样
    SCL_HIGH;
    I2C_DELAY;

    // 3. SDA保持不变、SCL拉低，结束信号采样
    SCL_LOW;
    I2C_DELAY;
}

// 主机等待从机发出响应
uint8_t I2C_Wait4Ack(void)
{
    // 1. SCL拉低、SDA拉高、主机释放数据总线 
    SCL_LOW;
    SDA_HIGH;
    I2C_DELAY;

    // 2. SCL拉高、开始信号采样 从机控制SDA，主机不用管其状态
    SCL_HIGH;
    I2C_DELAY;

    // 3. 获取采集的响应
    uint16_t ack = READ_SDA;  

    // 4. SCL拉低，结束信号采样 数据总线由从机控制，主机设备不用管SDA线上的情况
    SCL_LOW;
    I2C_DELAY;

    return ack ? NACK : ACK;
}

// 主机向从机写入一个字节的数据（发送）
void I2C_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. SCL拉低、SDA拉低，准备数据采样
        SDA_LOW;
        SCL_LOW;
        I2C_DELAY;

        // 2. 获取单字节数据最高位
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        I2C_DELAY;

        // 3. SCL拉高，开始数据采样
        SCL_HIGH;
        I2C_DELAY;

        // 4. SCL拉低，结束数据采样
        SCL_LOW;
        I2C_DELAY;

        // 5. 左移一位
        byte <<= 1;
    }
}

// 主机向从机读取一个字节的数据（接收）
uint8_t I2C_ReadByte(void)
{
    uint8_t byte = 0;
    
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. SCL拉低，等待数据翻转
        SCL_LOW;
        I2C_DELAY;

        // 2. SCL拉高，开始从机的数据采样
        SCL_HIGH;
        I2C_DELAY;

        // 3. 读取从机数据 
        byte <<= 1;
        if (READ_SDA)
        {
            byte |= 0x01; 
        }
        
        // 4. SCL拉低，结束数据采样
        SCL_LOW;
        I2C_DELAY;
    }
    
    return byte;
}
