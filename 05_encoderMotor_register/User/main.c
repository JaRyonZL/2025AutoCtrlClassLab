/*
 * @Descripttion: JGA25-370直流减速电机速度环demo
 * @Author: JaRyon
 * @version: v1.0
 * @Date: 2025-09-25 12:03:52
 */
#include "includes.h"

int main(void)
{
    BSPInit();

    printf("PID motor speed control test start...\r\n");

    PID_TypeDef pid;
    PID_Init(&pid, 2.1f, 0.8f, 0);
    pid.SetSpeed = 140.0f;

    while (1)
    {
        // PID 计算
        float pid_output = PID_Calc(&pid, rpm_global);

        // 设置电机速度
        JGA25_370_SetSpeed((int32_t)pid_output);

        // 串口输出
        printf("%.2f,%.2f,%.2f\r\n", pid.SetSpeed, rpm_global, pid_output);

        Delay_ms(20);
    }
}
