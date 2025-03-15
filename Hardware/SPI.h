#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

void M_SPI_Init(void);
void SPI_Start(void);
void SPI_Stop(void);
void SPI_Send(uint8_t data);
void SPI_W_DC(uint8_t value);

#endif /* __SPI_H__ */
