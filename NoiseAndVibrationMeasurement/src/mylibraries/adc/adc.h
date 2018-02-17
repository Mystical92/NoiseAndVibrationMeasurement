/*
 * adc.h
 *
 * Created: 23.01.2016 20:23:12
 *  Author: kamil
 */ 


#ifndef ADC_H_
#define ADC_H_

void ADC_configurate(void);
void adcCH_noiseMeasure_enable(void);
void adcCH_batteryMeasure_enable(void);
void adcTriggerTimer_enable(void);
void adcTriggerTimer_disable(void);

uint16_t adc_GetOffset(void);
uint16_t adc_MeasureBatteryVoltage(void);

#endif /* ADC_H_ */