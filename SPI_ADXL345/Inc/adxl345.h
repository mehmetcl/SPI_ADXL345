/*
 * adxl345.h
 *
 *  Created on: May 12, 2022
 *      Author: Admin
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include "spi.h"
#include "stm32f407xx_gpio.h"

#define INT_SOURCE_REGISTER		0x30
#define DATA_REGISTER 			0x32
#define READ_MULTI_BYTE			6




void adxl_init();
void adxl_write(uint8_t address, uint8_t data);
void adxl_read(uint8_t address,uint8_t *prxbuffer, uint8_t len);
void adxl_accelerometer_init(void);
void adxl_tap_init(void);

#endif /* ADXL345_H_ */
