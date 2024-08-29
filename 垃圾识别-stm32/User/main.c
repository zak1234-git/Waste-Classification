#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"
#include "Serial.h"

uint8_t KeyNum;
float Angle;

int main(void)
{
	Servo_Init();
	Serial_Init();
	Servo_SetAngle_Reset(); 

	while(1)
	{
		 
	}
}
