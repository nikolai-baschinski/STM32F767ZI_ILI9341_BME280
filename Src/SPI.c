#include "stm32f767xx.h"
#include "SPI.h"

void init_SPI()
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI1
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // SPI1 is served by DMA2 (RM0410 DMA2 request mapping, channel 3)

  // CPOL and CPHA is 0 by default (SPI mode 0)

  SPI1->CR1 &= ~(3<<3);
  SPI1->CR1 |= (3<<3); // divide SYSCLK by 16

  // MSB comes first, default setting

  SPI1->CR1 |= SPI_CR1_SSM; // Enable software slave management
  SPI1->CR1 |= SPI_CR1_SSI; // Set internal slave select to 1

  // Full-duplex, RXONLY-Bit is default 0

  SPI1->CR1 |= SPI_CR1_MSTR; // Set master mode

  SPI1->CR2 &= ~(0xF<<8);
  SPI1->CR2 |= (7<<8); // 8 bits data
  SPI1->CR2 |= SPI_CR2_FRXTH; // RXNE on every single byte, no packing

  SPI1->CR1 |= (1<<6); // Enable SPI
}

void spi_transfer(const uint8_t* tx_buf, uint8_t* rx_buf, uint8_t len)
{
  for(uint8_t i = 0; i < len; i++) {
    while(!(SPI1->SR & SPI_SR_TXE));
    *((__IO uint8_t*)&SPI1->DR) = tx_buf[i];

    while(!(SPI1->SR & SPI_SR_RXNE));
    uint8_t byte = *((__IO uint8_t*)&SPI1->DR);
    if(rx_buf != 0) {
      rx_buf[i] = byte;
    }
  }

  while(SPI1->SR & SPI_SR_BSY); // wait until the last bit has left the shift register
}

// Full-duplex transfer via DMA2: RX on Stream0, TX on Stream3, both Channel 3
// (RM0410 DMA2 request mapping table). The CPU only sets both streams up and
// then waits for the RX transfer-complete flag - no per-byte polling at all,
// so SCK runs with no software-induced gaps for the whole transfer.
void spi_transfer_dma(const uint8_t* tx_buf, uint8_t* rx_buf, uint16_t len)
{
  DMA2_Stream0->CR &= ~DMA_SxCR_EN;
  while(DMA2_Stream0->CR & DMA_SxCR_EN);
  DMA2_Stream3->CR &= ~DMA_SxCR_EN;
  while(DMA2_Stream3->CR & DMA_SxCR_EN);

  DMA2->LIFCR = DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CFEIF0
              | DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3;

  // RX stream: peripheral (SPI1->DR) -> memory (rx_buf), 8-bit, memory address increments
  DMA2_Stream0->PAR  = (uint32_t)&SPI1->DR;
  DMA2_Stream0->M0AR = (uint32_t)rx_buf;
  DMA2_Stream0->NDTR = len;
  DMA2_Stream0->CR = (3UL << DMA_SxCR_CHSEL_Pos) | DMA_SxCR_MINC; // DIR = peripheral-to-memory (0)

  // TX stream: memory (tx_buf) -> peripheral (SPI1->DR), 8-bit, memory address increments
  DMA2_Stream3->PAR  = (uint32_t)&SPI1->DR;
  DMA2_Stream3->M0AR = (uint32_t)tx_buf;
  DMA2_Stream3->NDTR = len;
  DMA2_Stream3->CR = (3UL << DMA_SxCR_CHSEL_Pos) | DMA_SxCR_DIR_0 | DMA_SxCR_MINC; // DIR = memory-to-peripheral

  SPI1->CR2 |= SPI_CR2_RXDMAEN; // arm the receiver before the first byte goes out
  DMA2_Stream0->CR |= DMA_SxCR_EN;
  DMA2_Stream3->CR |= DMA_SxCR_EN;
  SPI1->CR2 |= SPI_CR2_TXDMAEN; // this starts the actual clocking

  while(!(DMA2->LISR & DMA_LISR_TCIF0)); // wait until all bytes have been received
  while(SPI1->SR & SPI_SR_BSY);          // wait until the last bit has left the shift register

  SPI1->CR2 &= ~(SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
  DMA2_Stream0->CR &= ~DMA_SxCR_EN;
  DMA2_Stream3->CR &= ~DMA_SxCR_EN;
}
