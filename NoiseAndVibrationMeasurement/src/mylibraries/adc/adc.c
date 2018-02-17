/*
 * adc.c
 *
 * Created: 21.01.2016 21:16:12
 *  Author: kamil
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stddef.h>


#include "adc.h"

void adcCH_noiseMeasure_init(ADC_CH_t *adcch, register8_t muxpos, register8_t muxneg);
void adcCH_batteryMeasure_init(ADC_CH_t *adcch, register8_t muxpos);
void adcTriggerTimer_init();
uint8_t ReadCalibrationByte(uint8_t index);

void ADC_configurate() 
{
	PORTA.DIRCLR = PIN1_bm;

	ADCA.CTRLA=ADC_ENABLE_bm;
	// Rozdzielczoœæ 11 bitów, tryb ze znakiem
	ADCA.CTRLB=ADC_CONMODE_bm;
	// Referencja 1V
	ADCA.REFCTRL=ADC_REFSEL_AREFA_gc;
	// CLKADC=1 MHz
	ADCA.PRESCALER=ADC_PRESCALER_DIV32_gc;  
	// Kalibracja kana³ów ADC
	ADCA.CALL=ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
	ADCA.CALH=ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));

}

void adcCH_noiseMeasure_enable()
{
	adcTriggerTimer_init();
}

void adcCH_batteryMeasure_enable()
{
	adcCH_batteryMeasure_init(&ADCA.CH0, ADC_CH_MUXPOS_PIN2_gc);
}

void adcTriggerTimer_init() 
{
	// domyœlnie zegar nie podlaczony
	TCC4.CTRLA&=~TC4_CLKSEL_gm;
	
	// Taktowanie = 32[MHz] : 8 = 4[MHz]
	// 4 000 000[Hz] / PER + 1  => PER = 499
	// PER = 9 dla 50kHz
	TCC4.PER=499;
	TCC4.CCA=0;
}

void adcTriggerTimer_enable() 
{
	TCC4.CCA=0;
    // Prescaler = 64 dla 500kHz
	TCC4.CTRLA|=TC4_CLKSEL0_bm | TC4_CLKSEL2_bm; 

}

void adcTriggerTimer_disable() 
{
	TCC4.CTRLA&=~TC4_CLKSEL_gm;
}

uint16_t adc_GetOffset()
{
	uint32_t accum = 0;
	uint16_t counter = 0;

	do 
	{
		ADCA.CH0.CTRL |= ADC_CH_START_bm;
		while(!(ADCA.CH0.INTFLAGS & ADC_CH0IF_bm));
		
		ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;
		accum += ADCA.CH0RES;
		counter++;
	} while (counter < 1024);
	
	return accum / 1024;
}

uint16_t adc_MeasureBatteryVoltage()
{
	uint32_t accum = 0;
	uint16_t counter = 0;

	adcCH_batteryMeasure_enable();
	do
	{
		ADCA.CH0.CTRL |= ADC_CH_START_bm;
		while(!(ADCA.CH0.INTFLAGS & ADC_CH0IF_bm));
		
		ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;
		accum += ADCA.CH0RES;
		counter++;
	} while (counter < 1024);
	
	return accum / 1024;
}

uint8_t ReadCalibrationByte(uint8_t index)
{
	uint8_t result;
	// odczytaj sygnaturê produkcyjn¹
	NVM_CMD=NVM_CMD_READ_CALIB_ROW_gc;
	result=pgm_read_byte(index);
	// przywróæ normalne dzia³anie NVM
	NVM_CMD=NVM_CMD_NO_OPERATION_gc;
	return result;
}



