#include "stm32f4xx.h"

/**
 * @brief  Initializes the SPI.
 *  GPIOB Pin3 SPI_OLED D0 -> SPI_CLOCK
 *  GPIOB Pin4 SPI_OLED D1 -> SPI_MOSI
 *  GPIOB Pin5 SPI_OLED CS -> SPI_CS
 *  GPIOB Pin6 SPI_OLED DC -> Command/Data  (0:Command, 1:Data)
 *  GPIOB Pin7 SPI_OLED RES -> Reset
 */

void SPI_W_SCK(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)value);
}
void SPI_W_MOSI(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)value);
}
// uint8_t SPI_R_MISO(void) {
//     return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
// }
void SPI_W_CS(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)value);
}
void SPI_W_DC(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)value);
}
void SPI_W_RES(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)value);
}

void M_SPI_Init(void) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_W_CS(1);
    SPI_W_SCK(0);
    SPI_W_DC(1);
    SPI_W_RES(1);
}

void SPI_Start(void) {
    SPI_W_CS(0);
}
void SPI_Stop(void) {
    SPI_W_CS(1);
}

void SPI_Send(uint8_t data) {
    // SPI_Start();
    for (int i = 0; i < 8; i++) {
        SPI_W_MOSI((data >> (7 - i)) & 0x01);
        SPI_W_SCK(1);
        SPI_W_SCK(0);
    }
    // SPI_Stop();
}

// void SPI_Receive(uint8_t *data) {

// }
