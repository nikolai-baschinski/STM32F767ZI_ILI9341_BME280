#include "stm32f767xx.h"
#include "SPI.h"

void init_SPI()
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SP1

  // CPOL and CPHA is 0 by default (SPI mode 0)

  SPI1->CR1 &= ~(3<<3);
  SPI1->CR1 |= (3<<3); // divide SYSCLK by

  // MSB comes first, default setting

  SPI1->CR1 |= SPI_CR1_SSM; // Enable software slave management
  SPI1->CR1 |= SPI_CR1_SSI; // Set internal slave select to 1

  // Full-duplex, RXONLY-Bit is default 0

  SPI1->CR1 |= SPI_CR1_MSTR; // Set master mode

  SPI1->CR2 &= ~(0xF<<8);
  SPI1->CR2 |= (7<<8); // 8 bits data
  SPI1->CR2 |= SPI_CR2_FRXTH; // Read 8 bits

  SPI1->CR1 |= (1<<6); // Enable SPI
}

uint8_t spi_trancive_burst(uint8_t data, uint8_t* rcv_buf, uint8_t index)
{
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = data;
  while(SPI1->SR & SPI_SR_BSY);

  while((SPI1->SR & SPI_SR_RXNE) && (index < MAX_RECV_BURST)) {
    rcv_buf[index] = SPI1->DR;
    index++;
  }
  return index;
}

void spi_send(uint8_t byte)
{
  while(!(SPI1->SR & SPI_SR_TXE));
  *((__IO uint8_t*)&SPI1->DR) = byte;
  while(SPI1->SR & SPI_SR_BSY);

  while(SPI1->SR & SPI_SR_RXNE) {
    (void)SPI1->DR;
  }
}

uint8_t spi_send_recv(uint8_t data)
{
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = data;
  while(SPI1->SR & SPI_SR_BSY);

  #define MAX_RECV 2

  uint8_t data_rcv[MAX_RECV]= {0};
  uint8_t i = 0;
  while((SPI1->SR & SPI_SR_RXNE) && (i < MAX_RECV)) {
    data_rcv[i] = (SPI1->DR);
    i++;
  }
  return data_rcv[1];
}
