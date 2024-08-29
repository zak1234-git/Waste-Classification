#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//舵机初始化
void Servo_Init(void)
{
	PWM_Init();
}

// 舵机控制函数，根据接收到的数据调整舵机位置
void Servo_SetAngle(uint8_t servo)
{
    uint16_t pulse = (90 * 2000 / 180) + 500; // 根据角度计算脉冲宽度 (500-2500us)

    switch (servo)
    {
        case 0x72:
            TIM_SetCompare2(TIM2, pulse); // 控制舵机1
            break;
        case 0x68:
            TIM_SetCompare3(TIM2, pulse); // 控制舵机2
            break;
        case 0x6B:
            TIM_SetCompare1(TIM3, pulse); // 控制舵机3
            break;
        case 0x6F:
            TIM_SetCompare2(TIM3, pulse); // 控制舵机4
            break;
        default:
            break;
    }
}

void Servo_SetAngle_Reset(void)
{
    uint16_t pulse = (0 * 2000 / 180) + 500; // 根据角度计算脉冲宽度 (500-2500us)
    TIM_SetCompare2(TIM2, pulse); // 控制舵机1
    TIM_SetCompare3(TIM2, pulse); // 控制舵机2
    TIM_SetCompare1(TIM3, pulse); // 控制舵机3
    TIM_SetCompare2(TIM3, pulse); // 控制舵机4

}

