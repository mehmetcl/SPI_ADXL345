/*
 * adxl345.c
 *
 *  Created on: May 12, 2022
 *      Author: Admin
 */


#include "adxl345.h"


extern uint8_t data_rec[6];


void adxl_init()
{

	uint8_t device_id;
	adxl_read(0x00, &device_id, 1); // if the read data is 0xE5, device is ready

	if(device_id!=0xE5)
		exit(1);



	adxl_write(0x2D, 0); // Reset power control all bits

	// Setup ->
	adxl_write(0x2D, 0x08); // Measure bit 1, wake up bit 0, autosleep 0, sleep 0,
	//wake up frequency 8 hz bcs we are not using it does not matter



}

void adxl_write(uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address;  //adress|40 multibyte write
	data[1] = value;
	gpio_write_out_pin(GPIOA, GPIO_PIN_3, DISABLE);

	spi_send_data(SPI1, data, 2);

	gpio_write_out_pin(GPIOA, GPIO_PIN_3, ENABLE);



}

void adxl_read(uint8_t address, uint8_t *prxbuffer, uint8_t len)
{


	//adres gonderdigimde garbage value`yu direkt almak istiyorum dummy code olmadan
	//veri icin dummy code gondermem gerekiyor

	uint8_t dummy=0;

	address |= 0x80;  // read operation
	if(len>1)
		address |= 1u << READ_MULTI_BYTE;

	gpio_write_out_pin(GPIOA, GPIO_PIN_3, DISABLE);

	//address
	spi_send_data(SPI1, &address, 1);
	spi_rec_data(SPI1, prxbuffer, 1, FALSE); //garbage value,dummy

	//veri
	spi_rec_data(SPI1, prxbuffer, len, TRUE);


	gpio_write_out_pin(GPIOA, GPIO_PIN_3, ENABLE);

}

void adxl_accelerometer_init(void)
{
	adxl_write(0x31, 0x01); // +- 4g range, 4-wire SPI mode
}

void adxl_tap_init(void)
{
	// Z eksenini basma algilama icin etkinlestir
		adxl_write(0x2A, 0x01);

		// 62.5 mg/LSB 40 * .0625 = 2.5 g
		adxl_write(0x1D, 27);

		//en az 0.02s durmali ki tap olsun
		// Basma suresini ayarla. 625uS/LSB * 32 = 0.02s
		adxl_write(0x21, 27);

		//1.tap ve 2.tap arasindaki min sure
		//0.01s sonra 2. tapi yapabilirsin
		// Dokunma gecikmesini ayarla. 1.25ms/LSB * 80 = 0,01s
		adxl_write(0x22, 80);


		//window 2.tap icin uygun sure basladiktan sonra 0.01snde basmalisin
		// Dokunma gecikmesini ayarla. 1.25ms/LSB * 80 = 0,01s
		adxl_write(0x23, 240);


		// INT MAP`i INT1 olarak ayarla
		adxl_write(0x2F, 0x00);

		// Tekli ve Ikili basmayi aktif et
		adxl_write(0x2E, 0x60);
}
