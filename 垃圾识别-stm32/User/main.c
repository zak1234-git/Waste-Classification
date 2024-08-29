#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Servo.h"
#include "Serial.h"

int main(void)
{
	Servo_Init();
	Serial_Init();
	Servo_SetAngle_Reset(); 

	while(1)
	{
		 
	}
}
