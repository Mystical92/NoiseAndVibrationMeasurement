/*
 * peripherals_init.c
 *
 * Created: 08.01.2016 22:03:17
 *  Author: kamil
 */ 

 #include <avr/io.h>
 #include "../usart/usart.h"
 #include "peripherals.h"
 #include "LUFA/Drivers/Peripheral/TWI.h"

void setRGB_pins(ConfigurationRGB config, uint8_t red, uint8_t green, uint8_t blue );


void TWI_configurate(void)
{
	PORTE.PIN0CTRL = PORT_OPC_WIREDANDPULL_gc;
	PORTE.PIN1CTRL = PORT_OPC_WIREDANDPULL_gc;
	TWI_Init(&TWIE, TWI_BAUD_FROM_FREQ(400000));
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
	 PORTF_OUTSET=PIN3_bm;
	 PORTF_DIRSET=PIN3_bm;
	 // wlacz odbiornik oraz nadajnik USART
	 USARTF0.CTRLB=USART_RXEN_bm | USART_TXEN_bm;
	 // ramka 8 bitów, bez parzystoœci, 1 bit stopu
	 USARTF0.CTRLC=USART_CHSIZE_8BIT_gc;
	 usart_set_baudrate(&USARTF0, 115200, F_CPU);
	 // odblokuj przerwania odbiornika
	 USARTF0.CTRLA=USART_RXCINTLVL_HI_gc;
 }

 void MMA8451_DataReady_InteruptConfig()
 {
	 PORTE.DIRCLR = PIN2_bm;
	 PORTE.PIN2CTRL = PORT_ISC_FALLING_gc | PORT_OPC_PULLUP_gc;
	 PORTE.INT0MASK = PIN2_bm;
	 PORTE.INTCTRL = PORT_INT0LVL_HI_gc;
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
