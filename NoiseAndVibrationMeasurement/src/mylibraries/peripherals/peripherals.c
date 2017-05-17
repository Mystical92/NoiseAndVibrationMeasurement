/*
 * peripherals_init.c
 *
 * Created: 08.01.2016 22:03:17
 *  Author: kamil
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "../usart/usart.h"
#include "peripherals.h"
#include "../../LUFA/TWI_XMEGA.h"
#include "../mma854x/mma854x.h"

void setRGB_pins(ConfigurationRGB config, uint8_t red, uint8_t green, uint8_t blue );


void TWI_configurate(void)
{
	PORTC.PIN0CTRL = PORT_OPC_WIREDANDPULL_gc;
	PORTC.PIN1CTRL = PORT_OPC_WIREDANDPULL_gc;
	TWI_Init(&TWIC, TWI_BAUD_FROM_FREQ(400000));
}

void configurateInterrupts(void)
{
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_LOLVLEN_bm;
	MMA8451_DataReady_InteruptConfig();
}

 void enableOscillator_32mhz(void) 
 {
	  OSC.CTRL     =    OSC_RC32MEN_bm;       // w³¹czenie oscylatora 32MHz
	  while(!(OSC.STATUS & OSC_RC32MRDY_bm)); // czekanie na ustabilizowanie siê generatora
	  CPU_CCP      =    CCP_IOREG_gc;         // odblokowanie zmiany Ÿród³a sygna³u
	  CLK.CTRL     =    CLK_SCLKSEL_RC32M_gc; // zmiana Ÿród³a sygna³u na RC 32MHz
  }

 void USART_configurate()
 {
	 PORTC_OUTSET = PIN3_bm;
	 PORTC_DIRSET = PIN3_bm;

	 PORTC_OUTCLR = PIN2_bm;
	 PORTC_DIRCLR = PIN2_bm;
	 // wlacz odbiornik oraz nadajnik USART
	 USARTC0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	 // ramka 8 bitów, bez parzystoœci, 1 bit stopu
	 USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
	 usart_set_baudrate(&USARTC0, 115200, F_CPU);
	 // odblokuj przerwania odbiornika
	 USARTC0.CTRLA = USART_RXCINTLVL_HI_gc;
 }

 void MMA8451_DataReady_InteruptConfig()
 {
	 PORTD.DIRCLR = PIN2_bm;
	 PORTD.PIN2CTRL = PORT_ISC_FALLING_gc | PORT_OPC_PULLUP_gc;
	 PORTD.INTMASK = PIN2_bm; // int source
	 PORTD.INTCTRL = PORT_INTLVL_HI_gc; // priorytet przerwania
 }
 
 bool configurateMMA845x(void)
 {
	 TWI_configurate();
	 
	 bool isErrorOccurred[4] = {false,false,false,false};
	 
	 if (MMA845x_EnableHighPassFilterData() > 0)
		isErrorOccurred[0] = true;
	 
	 if (MMA845x_InterruptConfig() > 0)
		isErrorOccurred[1] = true;

	 if(MMA845x_SetDataRate(0) > 0)
		isErrorOccurred[2] = true;

	 if(MMA845x_ActiveMode() > 0)
		isErrorOccurred[3] = true;

	 return (isErrorOccurred[0] && isErrorOccurred[1] && isErrorOccurred[2] && isErrorOccurred[3]);
 }

 void rgbLed_loadConfig(ConfigurationRGB config)
 {
	config.rgbPort->DIRSET = config.pinRed | config.pinGreen | config.pinBlue;
	config.rgbPort->OUTSET = config.pinRed | config.pinGreen | config.pinBlue;
 }

 void setRGB_color(EColor color, ConfigurationRGB config)
 {
	switch(color)
	{
		case RED:
		{
			setRGB_pins(config,1,0,0);
			break;
		}
		case GREEN:
		{
			setRGB_pins(config,0,1,0);
			break;
		}
		case BLUE:
		{
			setRGB_pins(config,0,0,1);
			break;
		}
		case YELLOW:
		{
			setRGB_pins(config,1,1,0);
			break;
		}
		case OFF:
		{
			setRGB_pins(config,0,0,0);
			break;
		}
	}
	

 }

 void setRGB_pins(ConfigurationRGB config, uint8_t red, uint8_t green, uint8_t blue )
 {
	if (red)
		config.rgbPort->OUTCLR = config.pinRed;
	else
		config.rgbPort->OUTSET = config.pinRed;

	if (green)
		config.rgbPort->OUTCLR = config.pinGreen;
	else
		config.rgbPort->OUTSET = config.pinGreen;

	if (blue)
		config.rgbPort->OUTCLR = config.pinBlue;
	else
		config.rgbPort->OUTSET = config.pinBlue;
 }
