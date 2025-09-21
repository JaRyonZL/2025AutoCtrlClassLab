#include "includes.h"

// 定义显存缓冲区
uint8_t dispMemBuf[8][128] = {0};

// OLED初始化：参考商家给定初始化方案
void OLED_Init(void)
{
	// 1. i2c初始化
	I2C_Init();

	// 2. 上电延时100ms
	Delay_ms(100);

	// 3. 发送设置显示关闭命令：0xAE
	OLED_WriteCommand(0xAE);

	// 4. 发送设置显示时钟频率命令：0xD5,0x80
	OLED_WriteCommand(0xD5);
	OLED_WriteCommand(0x80);

	// 5. 发送set Multiplex Ratio命令：0xA8,0x3F
	OLED_WriteCommand(0xA8);
	OLED_WriteCommand(0x3F);

	// 6. 发送 set Display Offset命令：0xD3,0x00
	OLED_WriteCommand(0xD3);
	OLED_WriteCommand(0x00);

	// 7. 发送 set Display Start Line命令：0x40
	OLED_WriteCommand(0x40);

	// 8. 发送 set Segment Re-Map命令：0xA1
	OLED_WriteCommand(0xA1);

	// 9. 发送 Set COM Output Scan Direction命令：0xC8
	OLED_WriteCommand(0xC8);

	// 10. 发送 Set COM Pins Hardware Configuration命令：0xDA,0x12
	OLED_WriteCommand(0xDA);
	OLED_WriteCommand(0x12);

	// 11. 发送 Contrast Control命令：0x81,0xCF
	OLED_WriteCommand(0x81);
	OLED_WriteCommand(0xCF);

	// 12. 发送 Set Pre-Charge Period命令：0xD9,0xF1
	OLED_WriteCommand(0xD9);
	OLED_WriteCommand(0xF1);

	// 13. 发送 Set VCOMH Deselect Level命令：0xDB,0x30
	OLED_WriteCommand(0xDB);
	OLED_WriteCommand(0x30);

	// 14. 发送 Set Entire Display On/Off命令：0xA4
	OLED_WriteCommand(0xA4);

	// 15. 发送 Set Normal/Inverse Display命令：0xA6
	OLED_WriteCommand(0xA6);

	// 16. 发送 Set Charge Pump命令：0x8D,0x14
	OLED_WriteCommand(0x8D);
	OLED_WriteCommand(0x14);

	// 17. 发送 Set Display On命令：0xAF
	OLED_WriteCommand(0xAF);

	// 18. 延时100ms
	Delay_ms(100);
}

// 写入一字节命令
void OLED_WriteCommand(uint8_t command)
{
	// 1. 开始数据传输
	I2C_Start();

	// 2. 发送从机地址
	I2C_SendByte(0x78);
	// 等待从机应答
	I2C_Wait4Ack();

	// 3. 写入控制字节：非连续模式写命令 Co-0 D/C#-0
	I2C_SendByte(0x00);
	// 等待从机应答
	I2C_Wait4Ack();

	// 4. 写入一字节命令
	I2C_SendByte(command);
	// 等待从机应答
	I2C_Wait4Ack();

	// 5. 停止传输
	I2C_Stop();
}

// 写入一字节数据
void OLED_WriteData(uint8_t *data, uint8_t len)
{
	// 1. 开始数据传输
	I2C_Start();

	// 2. 发送从机地址
	I2C_SendByte(0x78);
	// 等待从机应答
	I2C_Wait4Ack();

	// 3. 写入控制字节：非连续模式下写入 Co-0 D/C#-1
	I2C_SendByte(0x40);
	// 等待从机应答
	I2C_Wait4Ack();

	for (uint8_t i = 0; i < len; i++)
	{
		// 4. 写入多字节命令
		I2C_SendByte(data[i]);
		// 等待从机应答
		I2C_Wait4Ack();
	}

	// 5. 停止传输
	I2C_Stop();
}

// 设置光标显示初始位置
void OLED_SetCursorPos(uint8_t x, uint8_t page)
{
	// 1. 设置起始地址：列地址和页地址
	OLED_WriteCommand(0x00 | (x & 0x0F));
	OLED_WriteCommand(0x10 | (x & 0xF0) >> 4);
	OLED_WriteCommand(0xB0 | page);
}

// 清屏
void OLED_ClearAll(void)
{
	// 128x8每个像素点全给0x00，即全部熄灭即清屏
	for (uint8_t i = 0; i < 8; i++)
	{
		for (uint8_t j = 0; j < 128; j++)
		{
			dispMemBuf[i][j] = 0x00;
		}
	}
}

// 局部清屏
void OLED_ClearArea(uint8_t col, uint8_t row, uint8_t width, uint8_t height)
{
	for (uint8_t i = row; i < row + height; i++)
	{
		for (uint8_t j = col; j < col + width; j++)
		{
			// 坐标(j, i) = 0 每下一行，清除位高一位
			dispMemBuf[i / 8][j] &= ~(0x01 << (i % 8));
		}
	}
}

// 更新显存数据
void OLED_Update(void)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		OLED_SetCursorPos(0, i);
		OLED_WriteData(dispMemBuf[i], 128);
	}
}

// 显示单个字符
void OLED_ShowChar(uint8_t col, uint8_t row, char ch, uint8_t fontSize)
{
	// 往指定位置写数据
	if (fontSize == 6) // 6x8像素
	{
		OLED_ShowImage(col, row, 8, 6, OLED_Font6x8[ch - ' ']);
	}
	else if (fontSize == 8) // 8x16像素
	{
		OLED_ShowImage(col, row, 16, 8, OLED_Font8x16[ch - ' ']);
	}
}

// 显示字符串
void OLED_ShowString(uint8_t col, uint8_t row, char *string, uint8_t fontSize)
{
	for (uint8_t i = 0; string[i] != '\0'; i++)
	{
		OLED_ShowChar(col + i * fontSize, row, string[i], fontSize);
	}
}

int OLED_Pow(int x, int y)
{
	return (int)pow((double)x, (double)y);
}

// 显示十进制数值
void OLED_ShowDecNum(uint8_t col, uint8_t row, int number, uint8_t length, uint8_t fontSize)
{
	// 显示正负号
	if (number >= 0)
	{
		OLED_ShowChar(col, row, '+', fontSize);
	}
	else if (number < 0)
	{
		// 负数则转成正数
		number = -number;
		OLED_ShowChar(col, row, '-', fontSize);
	}

	// 将数字借助ASCll码变成字符，然后显示在OLED上
	for (uint8_t i = 0; i < length; i++)
	{
		OLED_ShowChar(col + (i + 1) * fontSize, row, (number / OLED_Pow(10, length - 1 - i)) % 10 + 48, fontSize);
	}
}

// 显示单位十进制数值
void OLED_ShowDecNum1(uint8_t col, uint8_t row, int number, uint8_t length, uint8_t fontSize)
{
	// 将数字借助ASCll码变成字符，然后显示在OLED上
	for (uint8_t i = 0; i < length; i++)
	{
		OLED_ShowChar(col + (i + 1) * fontSize, row, (number / OLED_Pow(10, length - 1 - i)) % 10 + 48, fontSize);
	}
}

// 显示十进制小数
void OLED_ShowFloat(uint8_t col, uint8_t row, double number, uint8_t intLen, uint8_t fraLen, uint8_t fontSize)
{
	// 显示正负号
	if (number >= 0)
	{
		OLED_ShowChar(col, row, '+', fontSize);
	}
	else if (number < 0)
	{
		// 负数则转成正数
		number = -number;
		OLED_ShowChar(col, row, '-', fontSize);
	}

	// 2. 将小数分割为整数部分和小数部分
	int intNum = 0;
	int powNum = 0;
	int fraNum = 0;

	// 记录整数部分
	intNum = number;

	// 记录小数部分
	number -= intNum;
	// 计算小数部分显示位数转化为整数时要乘的10的倍数
	powNum = OLED_Pow(10, fraLen);
	// 计算显示的小数部分值，并四舍五入得到整数
	fraNum = round(number * powNum);

	// 3. 计算得出最终的整数部分
	intNum += fraNum / powNum;

	// 4. 屏幕显示最终的小数
	OLED_ShowDecNum1(col + fontSize, row, intNum, intLen, fontSize);
	OLED_ShowChar(col + (intLen + 1) * fontSize, row, '.', fontSize);
	OLED_ShowDecNum1(col + (intLen + 2) * fontSize, row, fraNum, fraLen, fontSize);
}

// 显示图像
void OLED_ShowImage(uint8_t col, uint8_t row, uint8_t height, uint8_t width, const uint8_t *image)
{
	// 刷新显示区域
	OLED_ClearArea(col, row, width, height);

	// 观察发现，随着高度变化，后部分的高度为8的部分图像依次连接
	for (uint8_t j = 0; j < (height - 1) / 8 + 1; j++)
	{
		// 观察发现随宽度增长（幅度即图像宽度）显示两页组成的高度为8的部分图像
		for (uint8_t i = 0; i < width; i++)
		{
			// 第 起始高度除8＋j页，宽度为2处开始
			// row 0-7时 page-0 8-15 page-1 16-23-2
			dispMemBuf[row / 8 + j][col + i] |= image[i + width * j] << row % 8; // 取低5位，左移3位
			// 第 起始高度除8＋j + 1页
			dispMemBuf[(row / 8) + 1 + j][col + i] |= image[i + width * j] >> (8 - (row % 8)); // 取高3位，右移5位
		}
	}
}

// 显示汉字
void OLED_ShowChinese(uint8_t col, uint8_t row, char *chinese)
{
	// 一个汉字的存储一般占3个字节，定义单个汉字的数组
	char singleChinese[4] = {0};
	uint8_t pChinese = 0;
	// 定义字模索引
	uint8_t pIndex = 0;

	// 1. 遍历汉字字串，拆分单个汉字
	for (uint8_t i = 0; chinese[i] != '\0'; i++)
	{
		singleChinese[pChinese] = chinese[i];
		pChinese++;

		if (pChinese >= 3) // 拆得一个完整汉字
		{
			// 清除单字指向值
			pChinese = 0;

			// 2. 匹配字模，获取索引
			for (pIndex = 0; strcmp(OLED_ChineseFont16x16[pIndex].index, " ") != 0; pIndex++)
			{
				// 与汉字字模比较，找到对应汉字则退出
				if (strcmp(OLED_ChineseFont16x16[pIndex].index, singleChinese) == 0)
				{
					break;
				}
			}
		}
		// 3. 根据索引显示汉字
		// 注意显示时需要的偏移量：每三次得到一个汉字索引，一个汉字宽度为16
		if (i >= 2)
			OLED_ShowImage(col + ((i + 1) / 3 - 1) * 16, row, 16, 16, OLED_ChineseFont16x16[pIndex].data);
	}
}

// 绘制点
void OLED_DrawPoint(uint8_t X, uint8_t Y)
{
	dispMemBuf[Y / 8][X] |= 0x01 << (Y % 8);
}
