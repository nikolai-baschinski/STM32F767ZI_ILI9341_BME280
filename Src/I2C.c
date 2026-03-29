#include "I2C.h"
//#include "TIM.h" // include for delay()
#include <string.h>

#define SLAVE_ADDRESS 0x76
#define MAX_RECV_BURST 8

uint8_t burst_rcv_buffer[MAX_RECV_BURST]= {0};

void init_I2C2()
{
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; // Enable bus for I2C2

  // The STM32CubeMX tool calculates and provides the I2C_TIMINGR content in the I2C configuration window.
  I2C2->TIMINGR = 0x20303e5d; // value for Standard Mode (rize time: 0, fall time: 0, coefficient of digital filter: 0)

  I2C2->CR1 |= I2C_CR1_PE; // Enable the peripheral
}

uint8_t cyclic_I2C2()
{
  return 0;
}

uint8_t* i2c2_trancive_burst(uint8_t reg_start, uint8_t length)
{
  memset(burst_rcv_buffer, 0, MAX_RECV_BURST);

  // WRITE first register address
  uint32_t temp_CR2 = 0;
  temp_CR2 |= (1 << I2C_CR2_NBYTES_Pos);
  temp_CR2 |= (SLAVE_ADDRESS << 1);
  temp_CR2 |= I2C_CR2_START;
  I2C2->CR2 = temp_CR2;

  while(!(I2C2->ISR & I2C_ISR_TXE));
  I2C2->TXDR = reg_start;
  while(!(I2C2->ISR & I2C_ISR_TC));

  // READ
  temp_CR2 = 0;
  temp_CR2 |= (length << I2C_CR2_NBYTES_Pos);
  temp_CR2 |= (SLAVE_ADDRESS << 1);
  temp_CR2 |= I2C_CR2_RD_WRN;
  temp_CR2 |= I2C_CR2_START;
  I2C2->CR2 = temp_CR2;

  for(uint8_t i = 0; i < length; i++) {
    while(!(I2C2->ISR & I2C_ISR_RXNE));
    burst_rcv_buffer[i] = I2C2->RXDR;
  }
  I2C2->CR2 |= I2C_CR2_STOP;
  while(I2C2->ISR & I2C_ISR_BUSY);

  return burst_rcv_buffer;
}

uint8_t i2c2_read_write(uint8_t data)
{
  uint8_t rv = 0;

  // WRITE register address
  uint32_t temp_CR2 = 0;
  temp_CR2 |= (1 << I2C_CR2_NBYTES_Pos);
  temp_CR2 |= (SLAVE_ADDRESS << 1);
  temp_CR2 |= I2C_CR2_START;
  I2C2->CR2 = temp_CR2;

  while(!(I2C2->ISR & I2C_ISR_TXE));
  I2C2->TXDR = data;
  while(!(I2C2->ISR & I2C_ISR_TC));

  // READ 1 byte
  temp_CR2 = 0;
  temp_CR2 |= (1 << I2C_CR2_NBYTES_Pos);
  temp_CR2 |= (SLAVE_ADDRESS << 1);
  temp_CR2 |= I2C_CR2_RD_WRN;
  temp_CR2 |= I2C_CR2_START;

  I2C2->CR2 = temp_CR2;

  while(!(I2C2->ISR & I2C_ISR_RXNE));
  rv = I2C2->RXDR;

  I2C2->CR2 |= I2C_CR2_STOP;

  while(I2C2->ISR & I2C_ISR_BUSY);

  return rv;
}

void i2c2_write(uint8_t *data, uint8_t len)
{
  uint32_t temp_CR2 = 0;
  temp_CR2 |= (len << I2C_CR2_NBYTES_Pos); // number of bytes
  temp_CR2 |= (SLAVE_ADDRESS << 1);                  // slave address
  temp_CR2 |= I2C_CR2_START;                         // generate START
  I2C2->CR2 = temp_CR2;

  for(uint8_t i = 0; i < len; i++) {
    while(!(I2C2->ISR & I2C_ISR_TXE));
    I2C2->TXDR = data[i];
  }
  while(!(I2C2->ISR & I2C_ISR_TC));

  I2C2->CR2 |= I2C_CR2_STOP;
  while(I2C2->ISR & I2C_ISR_BUSY);
}
