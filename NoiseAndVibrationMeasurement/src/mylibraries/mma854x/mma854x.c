/*
 * MMA854x.c
 *
 * Created: 20.11.2015 12:14:14
 *  Author: kamil
 */ 

#include "mma854x.h"
#include "../../LUFA/TWI_XMEGA.h"

#define DATA_STATUS		0x00
#define OUT_X_MSB		0x01
#define INT_SOURCE		0x0C
#define XYZ_DATA_CFG	0x0E
#define CTRL_REG1		0x2A
#define CTRL_REG2		0x2B
#define CTRL_REG3		0x2C
#define CTRL_REG4		0x2D
#define CTRL_REG5		0x2E

#define ACTIVE_MASK		0x01
#define ZYXDR_MASK		0x08
#define FREAD_MASK		0x02
#define DR_MASK			0x38
#define HPF_OUT_MASK	0x10

//Scale Mask
#define FS_MASK			0x03
#define FS_4G			0x01
#define FS_8G			0x02

// Interrupt Mask
#define PP_OD_MASK		0x01
#define INT_EN_DR_MASK	0x01
#define INT_CFG_DR_MASK	0x01
 
uint8_t MMA845x_Standby(void)
{
	uint8_t readData;
	uint8_t regAddress = CTRL_REG1;
	uint8_t timeoutMS = 10;
	uint8_t error;
	uint8_t isError = 0;
	
	error = TWI_ReadPacket(	&TWI_PORT, TWI_SLAVE_ADDRES, timeoutMS, 
							&regAddress, sizeof(regAddress), 
							&readData, sizeof(readData));
	
	if(error > 0)
	{
		isError = 1;
	}
	
	uint8_t sendData = readData & ~ACTIVE_MASK;
	error = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, timeoutMS, 
							&regAddress, sizeof(regAddress),
							&sendData, sizeof(sendData));

	if(error > 0)
	{
		isError = 1;
	}
	
	return isError;
}

uint8_t MMA845x_ActiveMode(void)
{
	uint8_t readData;
	uint8_t regAddress = CTRL_REG1;
	uint8_t error;
	uint8_t isError = 0;
		
	error = TWI_ReadPacket(	&TWI_PORT, TWI_SLAVE_ADDRES, 10, 
							&regAddress, sizeof(regAddress),
							&readData, sizeof(readData));
	
	if(error > 0)
	{
		isError = 1;
	}
	
	uint8_t writeData = readData | ACTIVE_MASK;
		
	error = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10,
							&regAddress, sizeof(regAddress),
							&writeData, 1);
	
	if(error > 0)
	{
		isError = 1;
	}
	
	return isError;
}

void MMA845x_2g_ActiveMode(void)
{
	uint8_t readData;
	uint8_t regAddress = XYZ_DATA_CFG;
	
	MMA845x_Standby();

	TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&readData, sizeof(readData));
	
	uint8_t sendData = readData & ~FS_MASK;
	
	TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));
	
}

void MMA845x_4g_ActiveMode(void)
{
	uint8_t readData;
	uint8_t regAddress = XYZ_DATA_CFG;
		
	MMA845x_Standby();

	TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&readData, sizeof(readData));

	uint8_t sendData = (readData & ~FS_MASK) | FS_4G;
	
	TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));
		
}

void MMA845x_8g_ActiveMode(void)
{
	uint8_t readData;
	uint8_t regAddress = XYZ_DATA_CFG;
	
	MMA845x_Standby();

	TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&readData, sizeof(readData));
	
	uint8_t sendData = (readData & ~FS_MASK) | FS_8G;
	
	TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));
	
}

uint8_t MMA845x_EnableHighPassFilterData(void)
{
	uint8_t readData;
	uint8_t regAddress = XYZ_DATA_CFG;
	uint8_t isError = 0;
	
	isError = MMA845x_Standby();

	isError = TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&readData, sizeof(readData));
	
	uint8_t sendData = readData | HPF_OUT_MASK;
	
	isError = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));

	return isError;
}

uint8_t MMA845x_InterruptConfig(void)
{
	uint8_t readData;
	uint8_t error;
	uint8_t isError = 0;
		
	if(MMA845x_Standby() > 0)
	{
		return isError = 1;
	}
	
	uint8_t regAddress = CTRL_REG1;
	
	error = TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&readData, sizeof(readData));
	
	if(error > 0)
		isError = 1;
		
	uint8_t sendData = readData & ~FREAD_MASK;
	
	error = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));
	
	if(error > 0)
		isError = 1;
		
	regAddress = CTRL_REG3;
	sendData = PP_OD_MASK;
	
	error = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));
	
	if(error > 0)
		isError = 1;
	
	regAddress = CTRL_REG4;
	sendData = INT_EN_DR_MASK;
		
	error = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));

	if(error > 0)
		isError = 1;
		
	regAddress = CTRL_REG5;
	sendData = INT_CFG_DR_MASK;
	
	error = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));

	if(error > 0)
		isError = 1;
		
	return isError;	
}

uint8_t MMA845x_ReadData(uint8_t readData[])
{
	uint8_t regAddress = DATA_STATUS;
	uint8_t dataStatusReg;
	uint8_t isError = 0;

	TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&dataStatusReg, sizeof(dataStatusReg));

	if(dataStatusReg & ZYXDR_MASK)
	{
		regAddress = OUT_X_MSB;
		TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
		readData, 6);
	}
	else
		isError = 1;

	return isError;
}

void MMA845x_readDataWithoutCheck(uint8_t readData[])
{
	uint8_t regAddress = OUT_X_MSB;

	TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 1, 
					&regAddress, sizeof(regAddress),
					readData, 6);
}

uint8_t MMA845x_SetDataRate(int8_t dataRate)
{
	if(dataRate > 7)
	{
		dataRate = 7;
		dataRate <<=3;
	}
	else
	{
		dataRate <<=3;
	}

	uint8_t readData;
	uint8_t regAddress = CTRL_REG1;
	uint8_t isError = 0;

	isError = MMA845x_Standby();

	isError = TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&readData, sizeof(readData));
	
	uint8_t sendData = readData & ~DR_MASK;

	isError = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));
		
	isError = TWI_ReadPacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&readData, sizeof(readData));

	sendData = dataRate | readData;

	isError = TWI_WritePacket(&TWI_PORT, TWI_SLAVE_ADDRES, 10, &regAddress, sizeof(regAddress),
	&sendData, sizeof(sendData));

	return isError;
}
