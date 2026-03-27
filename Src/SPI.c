#include "stm32f767xx.h"
#include "SPI.h"

// For the ILI9341 LCD
void init_SPI1()
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SP1

  // CPOL and CPHA is 0 by default (SPI mode 0)

  SPI1->CR1 |= (3<<3); // divide SYSCLK by

  // MSB comes first, default setting

  SPI1->CR1 |= SPI_CR1_SSM; // Enable software slave management
  SPI1->CR1 |= SPI_CR1_SSI; // Set internal slave select to 1

  // Full-duplex, RXONLY-Bit is default 0

  SPI1->CR1 |= SPI_CR1_MSTR; // Set master mode

  SPI1->CR2 |= (3<<8); // 8 bits data

  SPI1->CR1 |= (1<<6); // Enable SPI
}

// For the BDM280
void init_SPI2()
{
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // Enable SP2

  // CPOL and CPHA is 0 by default (SPI mode 0)

  SPI2->CR1 &= ~(7 << 3);
  SPI2->CR1 |=  (3 << 3); // divide SYSCLK by 16

  SPI2->CR1 |= SPI_CR1_SSM; // Enable software slave management
  SPI2->CR1 |= SPI_CR1_SSI; // Set internal slave select to 1

  // Full-duplex, RXONLY-Bit is default 0

  SPI2->CR1 |= SPI_CR1_MSTR; // Set master mode

  SPI2->CR2 |= (3<<8); // 8 bits data
  SPI2->CR2 |= SPI_CR2_FRXTH; // Read 8 bits

  SPI2->CR1 |= (1<<6); // Enable SPI
}

void init_SPI()
{
  init_SPI1();
  init_SPI2();
}

uint8_t spi2_trancive_burst(uint8_t data, uint8_t* rcv_buf, uint8_t index)
{
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = data;
  while(SPI2->SR & SPI_SR_BSY);

  while((SPI2->SR & SPI_SR_RXNE) && (index < MAX_RECV_BURST)) {
    rcv_buf[index] = SPI2->DR;
    index++;
  }
  return index;
}

void spi2_send(uint8_t byte)
{
  while(!(SPI2->SR & SPI_SR_TXE));
  *((__IO uint8_t*)&SPI2->DR) = byte;
  while(SPI2->SR & SPI_SR_BSY);
}

uint8_t spi2_send_recv(uint8_t data)
{
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = data;
  while(SPI2->SR & SPI_SR_BSY);

  #define MAX_RECV 2

  uint8_t data_rcv[MAX_RECV]= {0};
  uint8_t i = 0;
  while((SPI2->SR & SPI_SR_RXNE) && (i < MAX_RECV)) {
    data_rcv[i] = (SPI2->DR);
    i++;
  }
  return data_rcv[1];
}
