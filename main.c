/*
 * main.c
 */

#include "TM4C123GH6PM.h"
#include "bsp.h"

// period is 16-bit number of PWM clock cycles in one period (3 < = period)
// duty is number of PWM clock cycles output is high (2 < = duty < = period-1)
// PWM clock rate = processor clock rate/ SYSCTL_RCC_PWMDIV
// = BusClock/ 2 (in this example)

void PWM0_Init(uint16_t period, uint16_t duty){
	SYSCTL->RCGCPWM  |=  0x01;
	SYSCTL->RCGCGPIO |=  0x02;
	while((SYSCTL->PRGPIO & 0x02) == 0) { }
	GPIOB->AFSEL     |=  0x40;
	GPIOB->PCTL       = (GPIOB->PCTL & 0xF0FFFFFF) + 0x04000000;
	GPIOB->AMSEL     &= ~0x40;
	GPIOB->DEN       |=  0x40;
	SYSCTL->RCC       =  0x00100000 | (SYSCTL->RCC & (~0x000E0000));
	PWM0->_0_CTL      =  0x00;
	PWM0->_0_GENA     =  0xC8;
	PWM0->_0_LOAD     =  period - 1;
	PWM0->_0_CMPA     =  duty - 1;
	PWM0->_0_CTL     |=  0x00000001;
	PWM0->ENABLE     |=  0x00000001;
}

void PWM0_Duty(uint16_t duty) {
	PWM0->_0_CMPA = duty - 1;
}

int main(void) { uint16_t duty = 15000;
	PWM0_Init(30000, duty);
	while(1){
		PWM0->_0_CMPA = duty;
		if(duty > 0) {
			--duty;
		} else {
			duty = 15000;
		}
	}
}
