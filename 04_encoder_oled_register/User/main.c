/*
 * @Descripttion: 
 * @Author: JaRyon
 * @version: 
 * @Date: 2025-09-21 11:45:26
 */
/*
 * @Descripttion:
 * @Author: JaRyon
 * @version:
 * @Date: 2025-09-21 11:45:26
 */
// FileName: main.c

#include "includes.h"

// 定义当前值、上一次计数值、变化值
int16_t LastCnt = 0, CurrentCnt = 0, deltaCnt = 0;
// 编码器计数
int16_t Encoder_value = 0;  // 计数
double Encoder_angle = 0.0; // 旋转角度
int16_t Encoder_round = 0;  // 圈数

int main(void)
{
    OLED_Init();
    ENCODER_Init();

    while (1)
    {
        CurrentCnt = ENCODER_GetCnt();   // 获取当前计数值
        deltaCnt = CurrentCnt - LastCnt; // 计算计数变化值

        Encoder_value += deltaCnt; // 更新编码器值
        LastCnt = ENCODER_GetCnt();

        Encoder_angle = (int16_t)(Encoder_value / 80.0 * 360) % 360;
        Encoder_round = Encoder_value / 80;

        OLED_ShowString(3, 5, "cnt: ", 8);
        OLED_ShowDecNum(40, 5, Encoder_value, 5, 8);

        OLED_ShowString(3, 25, "angle: ", 8);
        OLED_ShowDecNum(55, 25, Encoder_angle, 3, 8);

        OLED_ShowString(3, 45, "round: ", 8);
        OLED_ShowDecNum(55, 45, Encoder_round, 2, 8);

        OLED_Update();
    }
}
