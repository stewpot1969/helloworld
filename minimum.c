#ifndef __NO_SYSTEM_INIT
void SystemInit()
{}
#endif

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

/// Note: USB serial doodah for GPS:
//  BLK = GND
//  RED = 5V
//  YEL = TXD
//  GRN = RXI


static void gpio_setup(void)
{

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO12|GPIO13|GPIO14|GPIO15);

	gpio_set_mode(GPIOC, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6|GPIO7|GPIO8);
		      
  gpio_set(GPIOC,GPIO6|GPIO7|GPIO8);
  
}

void dlay(int j) {
  int i;
  for (i=0;i<j;i++)
    __asm__("nop");
}


void main()
{
  uint16_t inbits;
  int i=100000;
	gpio_setup();

	while (1) {
	
    // read button and set delay accordingly
    inbits=GPIOC_IDR;
    if ((inbits & (1<<6))==0) {
      i=70000;      // KEY1
    }
    else if ((inbits & (1<<7))==0) {
      i=35000;      // KEY2
    }
    else if ((inbits & (1<<8))==0) {
      i=12000;      // KEY3
    }    

		/* Using API function gpio_toggle(): */
		gpio_set(GPIOB, GPIO13);	/* LED on/off */
    dlay(i);
		gpio_clear(GPIOB, GPIO12);	/* LED on/off */
    dlay(i);
		gpio_set(GPIOB, GPIO14);	/* LED on/off */
		dlay(i);
		gpio_clear(GPIOB, GPIO13);	/* LED on/off */
    dlay(i);
		gpio_set(GPIOB, GPIO15);	/* LED on/off */
    dlay(i);
		gpio_clear(GPIOB, GPIO14);	/* LED on/off */
    dlay(i);
		gpio_set(GPIOB, GPIO12);	/* LED on/off */
    dlay(i);
		gpio_clear(GPIOB, GPIO15);	/* LED on/off */
    dlay(i);
	}

}
