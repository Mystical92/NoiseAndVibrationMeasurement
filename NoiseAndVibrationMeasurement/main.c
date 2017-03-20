/*
 * NoiseAndVibrationMeasurement.c
 *
 * Created: 28.02.2017 22:12:05
 * Author : Kamil Sznejder
 */ 

#include <avr/io.h>

#include "src/mylibraries/usart/usart.h"
#include "src/mylibraries/peripherals/peripherals.h"



int main(void)
{
	enableOscillator_32mhz();
	USART_configurate();

    while (1) 
    {
    }
}

