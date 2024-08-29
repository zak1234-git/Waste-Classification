#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Servo.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

#define HEADER 0xAA
#define FOOTER 0x55
#define BUFFER_SIZE 256

void Serial_Init(void)
{
	//开启USART和GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//初始化GPIO引脚,初始化RX
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//初始化USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;		//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx;		//串口模式
	USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长
	USART_Init(USART1, &USART_InitStructure);
	
	//开启中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	USART_Cmd(USART1, ENABLE);

}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}


//中断函数
void USART1_IRQHandler(void)			 
{
    uint8_t com_data;
    static uint8_t RxCounter = 0;
    static uint8_t RxBuffer[10] = {0};  // 假设包最大长度为 10（包头 + 数据 + 包尾）
    static uint8_t RxState = 0;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志
        com_data = USART_ReceiveData(USART1);

        switch (RxState)
        {
            case 0: // 等待包头 0xAA
                if (com_data == 0xAA)
                {
                    RxState = 1;
                    RxBuffer[RxCounter++] = com_data; // 存储包头
                }
                break;

            case 1: // 接收数据和包尾
                RxBuffer[RxCounter++] = com_data;

                // 检查是否达到最大包长度或接收到包尾 0x55
                if (com_data == 0x55 || RxCounter >= sizeof(RxBuffer))
                {
                    // 处理完整数据包
                    Servo_SetAngle(RxBuffer[1]);

                    // 重置状态和计数器，准备接收下一个数据包
                    RxState = 0;
                    RxCounter = 0;
                }
                break;

            default: // 任何异常情况，重置状态
                RxState = 0;
                RxCounter = 0;
                break;
        }
    }
}
