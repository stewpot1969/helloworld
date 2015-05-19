#ifndef __NO_SYSTEM_INIT
void SystemInit()
{}
#endif

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

/// Note: USB serial doodah for GPS:
//  BLK = GND
//  RED = 5V
//  YEL = TXD
//  GRN = RXI
// /usr/share/gcc-arm-none-eabi/samples

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

static void usart_setup(void)
{
	/* Setup GPIO pin GPIO_USART1_TX/GPIO9 on GPIO port A for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup UART parameters. */
	// usart_set_baudrate(USART1, 38400);
	/* TODO usart_set_baudrate() doesn't support 24MHz clock (yet). */
	/* This is the equivalent: */
	USART_BRR(USART1) = (uint16_t)((24000000 << 4) / (38400 * 16));

	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_24mhz();

	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);
}

void dlay(int j) {
  int i;
  for (i=0;i<j;i++)
    __asm__("nop");
}


void main()
{
  uint16_t inbits;
  int i=80;
  int c='A';
  int j;
  
  clock_setup();
	gpio_setup();
  usart_setup();

  while (1) {
    //send char, flash LED1
    gpio_toggle(GPIOB,GPIO12);
    usart_send_blocking(USART1,c);
    for (j = 0; j < 80000; j++)	/* Wait a bit. */
			__asm__("NOP");
    gpio_toggle(GPIOB,GPIO12);
    
    //reset char, flash LED2
    if (++c > 'Z') {
      c='A';
      gpio_toggle(GPIOB,GPIO13);
      for (j = 0; j < 80000; j++)	/* Wait a bit. */
	  		__asm__("NOP");
      gpio_toggle(GPIOB,GPIO13);
    }
    
    // end of line, flash LED3
    if (--i == 0) {
      gpio_toggle(GPIOB,GPIO14);
      usart_send_blocking(USART1,'\r');
      usart_send_blocking(USART1,'\n');
      i=80;
      for (j = 0; j < 80000; j++)	/* Wait a bit. */
	  		__asm__("NOP");
      gpio_toggle(GPIOB,GPIO14);
    }
    
    // short delay
    for (j = 0; j < 400000; j++)	/* Wait a bit. */
			__asm__("NOP");
	}
}
