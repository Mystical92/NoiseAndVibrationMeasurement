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
	// Rozdzielczo�� 11 bit�w, tryb ze znakiem
	ADCA.CTRLB=ADC_CONMODE_bm;
	// Referencja 1V
	ADCA.REFCTRL=ADC_REFSEL_AREFA_gc;
	// CLKADC=1 MHz
	ADCA.PRESCALER=ADC_PRESCALER_DIV32_gc;  
	// Kalibracja kana��w ADC
	ADCA.CALL=ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
	ADCA.CALH=ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));

}

void adcCH_noiseMeasure_enable()
{
	adcCH_noiseMeasure_init(&ADCA.CH0, ADC_CH_MUXPOS_PIN0_gc, ADC_CH_MUXNEG_PIN1_gc);
	adcTriggerTimer_init();
}

void adcCH_batteryMeasure_enable()
{
	adcCH_batteryMeasure_init(&ADCA.CH0, ADC_CH_MUXPOS_PIN2_gc);
}

void adcTriggerTimer_init() 
{
	// zwyk�y tryb pracy timera
	TCC0.CTRLB=TC_WGMODE_NORMAL_gc;
	// domy�lnie zegar nie podlaczony
	TCC0.CTRLA=TC_CLKSEL_OFF_gc;
	
	// Taktowanie = 32[MHz] : 8 = 4[MHz]
	// Dla cz�stotliwo�ci pr�bkowania 8[kHz]
	// 4 000 000[Hz] / PER + 1  => PER = 499
	// PER = 9 dla 50kHz
	TCC0.PER=499;
	TCC0.CCA=0;
	// routowane do kana�u zdarze� nr 0 	
	EVSYS_CH0MUX=EVSYS_CHMUX_TCC0_CCA_gc;
}

void adcTriggerTimer_enable() 
{
	TCC0.CCA=0;
	TCC0.CTRLA=TC_CLKSEL_DIV8_gc; 
	// 64 dla 500kHz
}

void adcTriggerTimer_disable() 
{
	TCC0.CTRLA=TC_CLKSEL_OFF_gc;
}

void adcCH_noiseMeasure_init(ADC_CH_t *adcch, register8_t muxpos, register8_t muxneg) 
{
	 adcch->CTRL = ADC_CH_INPUTMODE_DIFF_gc;
	 // pin wej�cia dodatniego
	 adcch->MUXCTRL = muxpos | muxneg;
	 adcch->INTCTRL = ADC_CH_INTLVL_HI_gc | ADC_CH_INTMODE_COMPLETE_gc;
}

void adcCH_batteryMeasure_init(ADC_CH_t *adcch, register8_t muxpos)
{
	adcch->CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	// pin wej�cia dodatniego
	adcch->MUXCTRL = muxpos;
	adcch->INTCTRL = 0;
}

uint16_t adc_GetOffset()
{
	uint32_t accum = 0;
	uint16_t counter = 0;

	adcCH_noiseMeasure_init(&ADCA.CH0, ADC_CH_MUXPOS_PIN1_gc, ADC_CH_MUXNEG_PIN1_gc);
	do 
	{
		ADCA.CH0.CTRL |= ADC_CH_START_bm;
		while(!(ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm));
		
		ADCA.CH0.INTFLAGS = ADC_CH_CHIF_bm;
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
		while(!(ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm));
		
		ADCA.CH0.INTFLAGS = ADC_CH_CHIF_bm;
		accum += ADCA.CH0RES;
		counter++;
	} while (counter < 1024);
	
	return accum / 1024;
}

uint8_t ReadCalibrationByte(uint8_t index)
{
	uint8_t result;
	// odczytaj sygnatur� produkcyjn�
	NVM_CMD=NVM_CMD_READ_CALIB_ROW_gc;
	result=pgm_read_byte(index);
	// przywr�� normalne dzia�anie NVM
	NVM_CMD=NVM_CMD_NO_OPERATION_gc;
	return result;
}



