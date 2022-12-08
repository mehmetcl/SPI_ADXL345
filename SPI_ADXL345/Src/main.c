
#include <stdint.h>

#include "stm32f407xx_gpio.h"


#include "spi.h"
#include "adxl345.h"

/*      SPI
Data Sheet Page 50
SPI1
PA4(NSS)
PA5:SCLK
PA6:MISO
PA7:MOSI

*/

uint8_t data_rec[6];
int16_t x,y,z;
float xG, yG, zG;

uint8_t status;

int main(void)
{
	GPIO_Handle_t gpioLed12 = {GPIOD,{GPIO_PIN_12,GPIO_MODE_OUT,GPIO_SPEED_FAST,GPIO_PUPD_NO,GPIO_OTYPE_PP,0}};
	GPIO_Handle_t gpioLed13 = {GPIOD,{GPIO_PIN_13,GPIO_MODE_OUT,GPIO_SPEED_FAST,GPIO_PUPD_NO,GPIO_OTYPE_PP,0}};
	GPIO_Handle_t gpioLed14 = {GPIOD,{GPIO_PIN_14,GPIO_MODE_OUT,GPIO_SPEED_FAST,GPIO_PUPD_NO,GPIO_OTYPE_PP,0}};
	GPIO_Handle_t gpioLed15 = {GPIOD,{GPIO_PIN_15,GPIO_MODE_OUT,GPIO_SPEED_FAST,GPIO_PUPD_NO,GPIO_OTYPE_PP,0}};

	gpio_clock_en_di(GPIOD, ENABLE);

	gpio_init(&gpioLed12);
	gpio_init(&gpioLed13);
	gpio_init(&gpioLed14);
	gpio_init(&gpioLed15);

	GPIO_Handle_t gpioSCLK = {GPIOA,{GPIO_PIN_5,GPIO_MODE_ALTER,GPIO_SPEED_VFAST,GPIO_PUPD_NO,GPIO_OTYPE_PP,0}};
	GPIO_Handle_t gpioMISO = {GPIOA,{GPIO_PIN_6,GPIO_MODE_ALTER,GPIO_SPEED_VFAST,GPIO_PUPD_NO,GPIO_OTYPE_PP,0}};
	GPIO_Handle_t gpioMOSI = {GPIOA,{GPIO_PIN_7,GPIO_MODE_ALTER,GPIO_SPEED_VFAST,GPIO_PUPD_NO,GPIO_OTYPE_PP,0}};
	GPIO_Handle_t gpioCS = {GPIOA,{GPIO_PIN_3,GPIO_MODE_OUT,GPIO_SPEED_LOW,GPIO_PUPD_NO,GPIO_OTYPE_PP,0}};


	gpio_clock_en_di(GPIOA, ENABLE);

	alternate_func_config(GPIOA, GPIO_PIN_5, SPI1_AF);
	alternate_func_config(GPIOA, GPIO_PIN_6, SPI1_AF);
	alternate_func_config(GPIOA, GPIO_PIN_7, SPI1_AF);


	gpio_init(&gpioSCLK);
	gpio_init(&gpioMISO);
	gpio_init(&gpioMOSI);
	gpio_init(&gpioCS);


	gpio_write_out_pin(GPIOA, GPIO_PIN_3, ENABLE);

	SPI_Handle_t spi_test={SPI1,{SPI_DEV_MASTER,SPI_DIRECTION_2LINES,SPI_DATASIZE_8BIT,SPI_POLARITY_HIGH,SPI_PHASE_2EDGE,\
	SPI_SSM_ENABLE,SPI_BR_PR8,FIRST_BIT_MSB}};

	spi_init(&spi_test);

	spi_enable(spi_test.pSPIx);
	adxl_init();



	adxl_accelerometer_init();

//	adxl_tap_init();

	while(1)
	{

		  //Accelerometer
		  adxl_read(DATA_REGISTER, data_rec, 6);
		  x = ((data_rec[1]<<8)|data_rec[0]);
		  y = ((data_rec[3]<<8)|data_rec[2]);
		  z = ((data_rec[5]<<8)|data_rec[4]);

		  xG = x * .0087;
		  yG = y * .0087 ;
		  zG = z * .0087;



		  //TAP

		/*  adxl_read(INT_SOURCE_REGISTER, &status, 1);


		  if((status >> 5) & 0x01) // double tap control
		  {
			  gpio_write_out_pin(GPIOD, GPIO_PIN_12, DISABLE);
			  gpio_write_out_pin(GPIOD, GPIO_PIN_13, DISABLE);
			  gpio_write_out_pin(GPIOD, GPIO_PIN_14, DISABLE);
			  gpio_write_out_pin(GPIOD, GPIO_PIN_15, DISABLE);



		  }
		  else if((status >> 6) & 0x01) // single tap control
		  {

			  gpio_write_out_pin(GPIOD, GPIO_PIN_12, ENABLE);
			  gpio_write_out_pin(GPIOD, GPIO_PIN_13, ENABLE);
			  gpio_write_out_pin(GPIOD, GPIO_PIN_14, ENABLE);
			  gpio_write_out_pin(GPIOD, GPIO_PIN_15, ENABLE);

		  }*/




	}
}
