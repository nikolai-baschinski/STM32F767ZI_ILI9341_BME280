#include "stm32f767xx.h"
#include "GPIO.h"

void init_GPIO()
{
  GPIOA->MODER |= (2<<10) | (2<<12) | (2<<14); // Alternate functions for PA5 (SPI1_CLK), PA6 (SP1_MISO) and PA7 (SPI1_MOSI)
  GPIOA->MODER |= (1<<0); // PA0 as output for DC (LCD)
  GPIOB->MODER |= (1<<0); // PB0 as output (LED1)
  GPIOB->MODER |= (1<<12); // PB6 as output for CS (LCD)
  GPIOB->MODER |= (1<<18); // PB9 as output for RST (LCD)
  GPIOF->MODER |= (2<<0); // Alternate function for PF0 (I2C2_SDA)
  GPIOF->MODER |= (2<<2); // Alternate function for PF1 (I2C2_CLK)
  GPIOG->MODER |= (1<<0); // PG0 as output (Test-Pin)

  GPIOA->OSPEEDR |= (3<<0) | (3<<10) | (3<<12) | (3<<14); // high speed PA0 (SS), PA5 (SPI1_CLK), PA6 (SPI1_MISO) and PA7 (SPI1_MOSI)
  GPIOF->OSPEEDR |= (3<<0) | (3<<2); // High speed for PF0 (I2C2_SDA) and PF1 (I2C2_CLK)

  GPIOA->AFR[0] |= (5<<20) | (5<<24) | (5<<28); // Alternate function 5 for SPI1
  GPIOF->AFR[0] |= (4<<0) | (4<<4); // Alternate function 4 for I2C2

  GPIOF->OTYPER |= (1<<0) | (1<<1); // Open drain output for the I2C2

  GPIOF->PUPDR |= (1<<0) | (1<<2); // Pull-ups for I2C2
}

void GPIO_toggle_green_LED()
{
  GPIOB->ODR & 1 ? (GPIOB->BSRR = (1<<16)) : (GPIOB->BSRR = (1<<0));
}

void GPIO_toggle_Pin_PG0(void)
{
  if (GPIOG->ODR & (1U << 0)) {
    GPIOG->BSRR = (1U << (0 + 16)); // Reset PG0
  } else {
    GPIOG->BSRR = (1U << 0);        // Set PG0
  }
}
