#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#define MAX_RECV_BURST 10

void init_SPI();
uint8_t spi_trancive_burst(uint8_t data, uint8_t* rcv_buf, uint8_t index);
void spi_send(uint8_t byte);
uint8_t spi_send_recv(uint8_t data);

#endif /* SPI_H_ */
