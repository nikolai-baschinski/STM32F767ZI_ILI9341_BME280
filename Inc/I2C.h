#ifndef I2C_H_
#define I2C_H_

#include "stm32f767xx.h"

void init_I2C2();

void i2c2_write(uint8_t *data, uint8_t len);
uint8_t i2c2_read_write(uint8_t data);
uint8_t* i2c2_trancive_burst(uint8_t reg_start, uint8_t length);

#endif
