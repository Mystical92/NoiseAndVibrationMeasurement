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
#include "src/mylibraries/mma854x/mma854x.h"
#include "src/mylibraries/data_conversion/DataConversion.h"

#define VIBRATION_SAMPLE_REQUIRED 1000

typedef enum
{
	WAIT_FOR_ACTION = 0,

	VIBRATIONS_MEASURE_INIT = 1,
	VIBRATIONS_MEASURE = 2,
	VIBRATIONS_DATA_COLLECTED = 3,

	NOISE_MEASURE_INIT = 4,
	NOISE_DATA_READY = 5,
	SOUND_LEVEL_RMS_SAMPLE_CALC = 6,

	BATTERY_VOLTAGE_MEASURE = 7,

	MEASURE_DATA_SEND = 8,
	WAIT_FOR_CMD = 9,

	SEND_RESPONSE = 10,

	ERROR = 11,

}EProcessState;

void parseToCharArr(char* arr, uint16_t number);
void sendString_with_float(	USART_t * const usart, float floatNumber, const char *txt_before, const char *txt_after );
void convertAndCalculateVibrationData(uint8_t *MMA845x_recivedValues, accelerationAxis_floatRepresentation *tempData, uint16_t *vibrationSampleCount);

volatile EProcessState global_procesState = VIBRATIONS_MEASURE_INIT;
volatile uint16_t errorCounter = 0;

uint8_t MMA845x_recivedValues[6];
uint16_t vibrationSampleCount;
accelerationAxis_floatRepresentation trueRmsVibrationData;


int main(void)
{
	enableOscillator_32mhz();
	//configurateInterrupts();
	USART_configurate();

	USART_send(&USARTC0, "PROGRAM START\n");
	
	USART_send(&USARTC0, "NEXT: configurateMMA845x\n");
	if(!configurateMMA845x())
		USART_send(&USARTC0, "CONFIG OK\n");
	else
		USART_send(&USARTC0, "CONFIG NOK\n");

	//sei();
	while (1)
	{
		
		switch(global_procesState)
		{
			case WAIT_FOR_CMD:
			{
				//setRGB_color(RED, myConfig);

				//if(isPacket(bufor, &recBuf))
				//{
					//setRGB_color(YELLOW, myConfig);
					//executeCommand(pkt);
				//}
				break;
			}

			case VIBRATIONS_MEASURE_INIT:
			{
				USART_send(&USARTC0, "VIBRATIONS_MEASURE_INIT\n");
				
				vibrationSampleCount = 0;
				trueRmsVibrationData.x = 0;
				trueRmsVibrationData.y = 0;
				trueRmsVibrationData.z = 0;
		
				MMA845x_ActiveMode();
				_delay_ms(5); // odfiltrowanie	;
				
				global_procesState = VIBRATIONS_MEASURE;
				
				//setRGB_color(BLUE, myConfig);
				break;
			}
			case VIBRATIONS_MEASURE:
			{
				//USART_send(&USARTC0, "VIBRATIONS_MEASURE\n");
							
				if(vibrationSampleCount <= VIBRATION_SAMPLE_REQUIRED)
				{
					MMA845x_ReadData(MMA845x_recivedValues);
					//MMA845x_readDataWithoutCheck(MMA845x_recivedValues);
					vibrationSampleCount++;
					convertAndCalculateVibrationData(MMA845x_recivedValues, &trueRmsVibrationData ,&vibrationSampleCount);
					global_procesState = VIBRATIONS_MEASURE;
				}
				else
				{
					global_procesState = VIBRATIONS_DATA_COLLECTED;
				}
				
				//setRGB_color(BLUE, myConfig);
				break;
			}
			case VIBRATIONS_DATA_COLLECTED:
			{	
				USART_send(&USARTC0, " VIBRATIONS_DATA_COLLECTED\n");
				//uint8_t MMA845x_recivedValues[6];

				//MMA845x_readDataWithoutCheck(MMA845x_recivedValues);

				//convertAndCalculateVibrationData(MMA845x_recivedValues, &trueRmsVibrationData ,&vibrationSampleCount);
				global_procesState = MEASURE_DATA_SEND;

					
				break;		
			}
			case NOISE_MEASURE_INIT:
			{
				USART_send(&USARTC0, "NOISE_MEASURE_INIT\n");
				//USART_send(&USARTF0, "Proces state: NOISE_MEASURE_INIT\n");
				//setRGB_color(YELLOW, myConfig);
				//microphoneVoltageSampleCount = 0;

				//trueRMS_microphoneVoltage_sampleCount = 0;
				//trueRMS_microphoneVoltageSamples[trueRMS_microphoneVoltage_sampleCount] = 0;

				//global_actualMeasurement = MICROPHONE_VOLTAGE_MEASUREMENT;
				//adcCH_noiseMeasure_enable();
				//adcTriggerTimer_enable();
				//
				//global_procesState = WAIT_FOR_ACTION;
				break;
			}
			case NOISE_DATA_READY:
			{
				USART_send(&USARTC0, "NOISE_DATA_READY\n");
				//voltageSamples_binary[microphoneVoltageSampleCount] = ADCA.CH0RES - (-5);
				//microphoneVoltageSampleCount++;

				//if(microphoneVoltageSampleCount == SOUND_LEVEL_SAMPLE_REQUIRED)
				//{	
					//adcTriggerTimer_disable();
					//global_procesState = SOUND_LEVEL_RMS_SAMPLE_CALC;
				//}
				//else
				//{
					//global_procesState = WAIT_FOR_ACTION;
				//}

				break;
			}
			case SOUND_LEVEL_RMS_SAMPLE_CALC:
			{	
				USART_send(&USARTC0, "SOUND_LEVEL_RMS_SAMPLE_CALC\n");
				//float realVoltageSample = 0;
					//
				//for (uint16_t sampleCount = 0; sampleCount <= SOUND_LEVEL_SAMPLE_REQUIRED; sampleCount++)
				//{
					//realVoltageSample = (voltageSamples_binary[sampleCount] / 2047.0f);
//
					//if(calculate_trueRMS_float(	&realVoltageSample, 
												//&trueRMS_microphoneVoltageSamples[trueRMS_microphoneVoltage_sampleCount],
												//&sampleCount,
												//SOUND_LEVEL_SAMPLE_REQUIRED ))
					//{	
						////sendString_with_float(	&USARTF0,
												////trueRMS_microphoneVoltageSamples[trueRMS_microphoneVoltage_sampleCount],
												////"TrueRMS value: ",
												////"[V]\n"
												////);										
					//}
				//}

				/*
				char resultValue_string[9];

				for(uint16_t sampleCount = 0; sampleCount < microphoneVoltage_sampleReq; sampleCount++)
				{
					realVoltageSample = (voltageSamples_binary[sampleCount] / 2047.0f);
					dtostrf(realVoltageSample, 9, 6, resultValue_string);
					USART_send_block(&USARTF0, resultValue_string, sizeof(resultValue_string));
					USART_send(&USARTF0, "\n ");
				}
				*/

				//trueRMS_microphoneVoltage_sampleCount++;
//
				//if(trueRMS_microphoneVoltage_sampleCount < SOUND_LEVEL_RMS_SAMPLE_REQUIRED)
				//{
					//trueRMS_microphoneVoltageSamples[trueRMS_microphoneVoltage_sampleCount] = 0;
					//microphoneVoltageSampleCount = 0;
//
					//global_procesState = WAIT_FOR_ACTION;
					//adcTriggerTimer_enable();
				//}
				//else
				//{
					//setRGB_color(GREEN, myConfig);
					//global_procesState = MEASURE_DATA_SEND;
				//}

				break;
			}
			case MEASURE_DATA_SEND:
			{			
				USART_send(&USARTC0, "MEASURE_DATA_SEND\n");	
				USART_send(&USARTC0, "\nVibration result: \n"); 
				sendString_with_float(&USARTC0, trueRmsVibrationData.x, "Xrms = ", "[g]\n");
				sendString_with_float(&USARTC0, trueRmsVibrationData.y, "Yrms = ", "[g]\n");
				sendString_with_float(&USARTC0, trueRmsVibrationData.z, "Zrms = ", "[g]\n");
							
				//float soundLevelMeasureResult = convert_rmsVoltage_to_dbSPL(
				//										trueRMS_microphoneVoltageSamples,
				//										SOUND_LEVEL_RMS_SAMPLE_REQUIRED,
				//										OP_AMP_GAIN, MICROPHONE_SENSITIVITY_IN_VOLTS );

				//if (soundLevelMeasureResult > 65.0)
				//{
					//sendString_with_float(&USARTF0, soundLevelMeasureResult,"\rSPL sound level =  ", "[dB]");
				//}
				//else
				//{
					//sendString_with_float(&USARTF0, 65,						"\rSPL sound level = >", "[dB]");
				//}
//
				//USART_send(&USARTF0, "\n\n");

				//sendMeasureDataToReciver(	trueRmsVibrationData.x,
											//trueRmsVibrationData.y,
											//trueRmsVibrationData.z,
											//0/*soundLevelMeasureResult*/);

				//global_procesState = WAIT_FOR_CMD;
				global_procesState = VIBRATIONS_MEASURE_INIT;
				break;
			}
			case SEND_RESPONSE:
			{
				USART_send(&USARTC0, "SEND_RESPONSE\n");	
				//setRGB_color(GREEN, myConfig);
				//sendCommand(DEVICE_READY_RESPONSE,0x00);

				global_procesState = WAIT_FOR_CMD;
				break;
			}
			case BATTERY_VOLTAGE_MEASURE:
			{
				USART_send(&USARTC0, "BATTERY_VOLTAGE_MEASURE\n");	
				//adcCH_batteryMeasure_enable();
				//ADCA.CTRLA |= ADC_CH0START_bm;
				//int16_t result = ADCA_CH0RES;
				//actualBatteryVoltage = result / 2048;

				break;
			}
			case ERROR:
			{
				USART_send(&USARTC0, "ERROR\n");	
				//setRGB_color(RED, myConfig);

				global_procesState = VIBRATIONS_MEASURE_INIT;
				break;
			}
			default:
			{
				
				break;
			}
		}
	}
}

ISR(PORTD_INT_vect)
{

}

void parseToCharArr(char* arr, uint16_t number)
{
	sprintf(arr, "%d", number);
}

void sendString_with_float(	USART_t * const usart,
							float floatNumber,
							const char *txt_before,
							const char *txt_after )
{
	char floatNumber_string[7];
	dtostrf( floatNumber, 6, 2, floatNumber_string);

	USART_send(usart, txt_before);
	USART_send_block(usart, floatNumber_string, sizeof(floatNumber_string));
	USART_send(usart, txt_after);
}

void convertAndCalculateVibrationData(uint8_t *MMA845x_recivedValues,
									  accelerationAxis_floatRepresentation *tempData,
									  uint16_t *vibrationSampleCount)
{
	accelerationAxis_binaryRepresentation rawValues;

	rawValues.x = ((int16_t)(MMA845x_recivedValues[0]<<8 | MMA845x_recivedValues[1])) >> 4;
	rawValues.y = ((int16_t)(MMA845x_recivedValues[2]<<8 | MMA845x_recivedValues[3])) >> 4;
	rawValues.z = ((int16_t)(MMA845x_recivedValues[4]<<8 | MMA845x_recivedValues[5])) >> 4;

	float x_realValue = convert_binary_to_acceleration_2g_mode(&rawValues.x);
	float y_realValue = convert_binary_to_acceleration_2g_mode(&rawValues.y);
	float z_realValue = convert_binary_to_acceleration_2g_mode(&rawValues.z);

	calculate_trueRMS_float(&x_realValue, &tempData->x, vibrationSampleCount, VIBRATION_SAMPLE_REQUIRED);
	calculate_trueRMS_float(&y_realValue, &tempData->y, vibrationSampleCount, VIBRATION_SAMPLE_REQUIRED);
	calculate_trueRMS_float(&z_realValue, &tempData->z, vibrationSampleCount, VIBRATION_SAMPLE_REQUIRED);
	
	(*vibrationSampleCount)++;
}

