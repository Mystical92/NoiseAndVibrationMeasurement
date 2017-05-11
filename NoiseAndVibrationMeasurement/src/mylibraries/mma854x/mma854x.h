/*
 * MMA854x.h
 *
 * Created: 20.11.2015 12:00:25
 *  Author: kamil
 */ 


#ifndef mma854x_H_
#define mma854x_H_

#include <stdint.h>

#define  TWI_PORT TWIC
#define  TWI_SLAVE_ADDRES 0x3A // adress 0x1D -> 11101<<1 = 111010 (0x3A)

typedef struct {
	int16_t x;	
	int16_t y;
	int16_t z;
} accelerationAxis_binaryRepresentation;

typedef struct {
	float x;
	float y;
	float z;
} accelerationAxis_floatRepresentation;

uint8_t MMA845x_Standby(void);

uint8_t MMA845x_ActiveMode(void);

void MMA845x_2g_ActiveMode(void);

void MMA845x_4g_ActiveMode(void);

void MMA845x_8g_ActiveMode(void);

uint8_t MMA845x_EnableHighPassFilterData(void);

uint8_t MMA845x_SetDataRate(int8_t);

void MMA845x_SetHighPassFilterFreq(void);

uint8_t MMA845x_InterruptConfig(void);

uint8_t MMA845x_ReadData(uint8_t[]);

void MMA845x_readDataWithoutCheck(uint8_t[]);

#endif /* INCFILE1_H_ */