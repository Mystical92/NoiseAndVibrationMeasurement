/*
 * DataConversion.c
 *
 * Created: 13.01.2016 20:48:06
 *  Author: kamil
 */ 

 #include <avr/io.h>
 #include <math.h>

 #include "DataConversion.h"

void FloatToByteArray(uint32_t* data, uint8_t byteArray[4])
{
	byteArray[0] = ((*data & 0xFF000000)>>24);
	byteArray[1] = ((*data & 0x00FF0000)>>16);
	byteArray[2] = ((*data & 0x0000FF00)>>8);
	byteArray[3] = ((*data & 0x000000FF)>>0);
}

void ByteArrayToFloat(uint8_t byteArray[4], uint32_t* dataOut)
{
	*dataOut =  ((uint32_t)byteArray[0]<<24) | ((uint32_t)byteArray[1]<<16) |
			    ((uint32_t)byteArray[2]<<8)  | ((uint32_t)byteArray[3]<<0);
}

float convert_binary_to_acceleration_2g_mode(int16_t *rawData)
{
	return (float)*rawData / 1024;
}

float convert_rmsVoltage_to_dbSPL(	float *rmsVoltage_array, uint16_t arraySize,
									double opAmpGain, double sensitivity_inVolts)
{
	double result = calculate_arithmeticMean(rmsVoltage_array, arraySize);
	double pascalPresure = result / opAmpGain / sensitivity_inVolts;

	return (float)20*log10(pascalPresure/0.00002);
}


double calculate_arithmeticMean(float *numbersArray, uint16_t arraySize)
{
	double accumulator = 0;
	
	for (uint16_t i = 0; i < arraySize; i++)
	{
		accumulator += numbersArray[i];
	}
	return accumulator / (double)arraySize;
}

uint8_t calculate_trueRMS_float(	float *newData,
									float *actualResult,
									uint16_t *sampleCount,
									uint16_t sampleCountRequired)
{
	if(*sampleCount < sampleCountRequired)
	{
		*actualResult += (*newData) * (*newData);
		return 0;
	}
	else
	{
		*actualResult = sqrt( *actualResult / (float)(sampleCountRequired + 1) );
		return 1;
	}
 }

uint8_t calculate_trueRMS_binary(	int16_t *newData,
									int32_t *actualResult_binary,
									uint16_t sampleCount,
									uint16_t sampleCountRequired)
{
	if(sampleCount < sampleCountRequired)
	{
		*actualResult_binary += ((int32_t)(*newData) * (int32_t)(*newData));
		return 0;
	}
	else
	 {
		*actualResult_binary = sqrt(*actualResult_binary/sampleCountRequired);
		return 1;
	}
}