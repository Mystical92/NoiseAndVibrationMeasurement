/*
 * DataConversion.h
 *
 * Created: 13.01.2016 20:48:22
 *  Author: kamil
 */ 


#ifndef DATACONVERSION_H_
#define DATACONVERSION_H_

typedef union FloatLongUnion{
	float f;
	uint32_t l;
}FloatLongUnion;

void FloatToByteArray(uint32_t* dataIn, uint8_t byteArray[4]);

void ByteArrayToFloat(uint8_t byteArray[4], uint32_t* dataOut);

float convert_binary_to_acceleration_2g_mode(int16_t *binaryData);

float convert_rmsVoltage_to_dbSPL(	float *rmsVoltage_array,
									uint16_t arraySize,
									double opAmpGain,
									double sensitivity_inVolts);

double calculate_arithmeticMean(float *numbersArray, uint16_t arraySize);

uint8_t calculate_trueRMS_float(	float *newData,
									float *actualResult,
									uint16_t *sampleCount,
									uint16_t sampleCountRequired);

uint8_t calculate_trueRMS_binary(	int16_t *newData,
									int32_t *actualResult_binary,
									uint16_t sampleCount,
									uint16_t sampleCountRequired);

#endif /* DATACONVERSION_H_ */