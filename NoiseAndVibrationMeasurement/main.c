/*
 * NoiseAndVibrationMeasurement.c
 *
 * Created: 28.02.2017 22:12:05
 * Author : Kamil Sznejder
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "src/mylibraries/usart/usart.h"
#include "src/mylibraries/peripherals/peripherals.h"



int main(void)
{
	enableOscillator_32mhz();
	USART_configurate();

	USART_send(&USARTC0, "Start\n");

	uint16_t counter = 0;
	while (1)
	{
		char str[15];
		sprintf(str, "%d", counter);
		counter++;
		USART_send(&USARTC0, str);
		USART_send(&USARTC0, ". ");
		USART_send(&USARTC0, "while Loop Start\n");
		_delay_ms(2000);
		USART_send(&USARTC0, "while Loop End\n");
	}
}

