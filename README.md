BME280 data (temperature, air pressure and humidity) displayed on the Waveshare ILI9341 LCD by STM32F767ZI (dev board) bare metal (no HAL) using the CubeIDE.

![BME280 and 2.4" LCD](/Doc/BME280_and_2.4_inch_LCD_controlled_by_STM32F767ZI.bmp)

BME280 and 2.4" LCD

Connections (https://os.mbed.com/platforms/ST-Nucleo-F767ZI/)


| LCD      | Port     | Function     |
|----------|----------|------------- |
| VCC      | 3,3V     | Vcc          |
| GND      | GND      | GND          |
| DIN      | PA7      | SPI1_MOSI    |
| CLK      | PA5      | SPI1_CLK     |
| CS       | PB6      | Chip Select  |
| DC       | PA0      | Data/Command |
| RST      | PB9      | Reset        |
| BL       | -        | Backlight    |


| BME280   | Port     | Function     |
|----------|----------|--------------|
| VCC      | 3,3V     | Vcc          |
| GND      | GND      | GND          |
| SCL      | PF1      | Clock        |
| SDA      | PF0      | Data         |
| ADDR     | not conn.|              |
| CS       | 3,3V     | Vcc          |


![BME280 raw data burst via SPI](/Doc/I2C_burst_read.bmp)

BME280 raw data burst via SPI

![Software_architecture](/Doc/Software_architecture.bmp)

Software architecture

Hardware
BME280: https://seengreat.com/product/207/bme280-environmental-sensor?srsltid=AfmBOorvlymsT9w0Ea-JBnftBbgADYcXMKpadnPHUyHl7X1wOO5TTgUa

LCD: https://www.waveshare.com/wiki/2.4inch_LCD_Module?srsltid=AfmBOoqtv3bq-mZfPtsi2BxiewwQnIkomXrloIzpVwGw_HnrOcmvQZar

Nucleo-STM32767ZI: https://www.st.com/en/evaluation-tools/nucleo-f767zi.html


Video: https://youtube.com/shorts/LbtwaH8bDvs?si=J_LBT7NFOr57htwr
