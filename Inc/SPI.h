#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void init_SPI();
void spi_transfer(const uint8_t* tx_buf, uint8_t* rx_buf, uint8_t len);
void spi_transfer_dma(const uint8_t* tx_buf, uint8_t* rx_buf, uint16_t len);
void spi_send(uint8_t byte);
uint8_t spi_send_recv(uint8_t data);

#endif /* SPI_H_ */
