#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
    // 开启 TIM2 和 TIM3 定时器的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 初始化 GPIO 引脚 (PA1, PA2 for TIM2, PA6, PA7 for TIM3)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // PA1 (TIM2_CH2), PA2 (TIM2_CH3)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA6 (TIM3_CH1), PA7 (TIM3_CH2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 TIM2 定时器
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;    // 自动重装载值（20ms周期）
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;    // 预分频系数 (1 MHz时钟)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 配置 TIM3 定时器
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 配置 TIM2 通道2 和通道3
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = ENABLE;

    TIM_OCInitStructure.TIM_Pulse = 0; // 设置初始占空比
    TIM_OC2Init(TIM2, &TIM_OCInitStructure); // PA1 - TIM2 CH2
    TIM_OC3Init(TIM2, &TIM_OCInitStructure); // PA2 - TIM2 CH3

    // 配置 TIM3 通道1 和通道2
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure); // PA6 - TIM3 CH1
    TIM_OC2Init(TIM3, &TIM_OCInitStructure); // PA7 - TIM3 CH2

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}
