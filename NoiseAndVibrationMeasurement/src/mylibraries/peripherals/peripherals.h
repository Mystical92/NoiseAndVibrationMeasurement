/*
 * peripherals_init.h
 *
 * Created: 08.01.2016 22:03:33
 *  Author: kamil
 */ 


#ifndef PERIPHERALS_INIT_H_
#define PERIPHERALS_INIT_H_

typedef enum
{
	OFF,
	RED,
	GREEN,
	BLUE,
	YELLOW,
}EColor;

typedef struct
{
	PORT_t* rgbPort;
	uint8_t pinRed;
	uint8_t pinGreen;
	uint8_t pinBlue;
}ConfigurationRGB;

void configurateInterrupts(void);
void enableOscillator_32mhz(void);
void USART_configurate(void);
void MMA8451_DataReady_InteruptConfig(void);
bool configurateMMA845x(void);
void TWI_configurate(void);


void rgbLed_loadConfig(ConfigurationRGB config);
void setRGB_color(EColor color, ConfigurationRGB config);

#endif /* PERIPHERALS_INIT_H_ */