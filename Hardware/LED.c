#include "stm32f4xx.h"
#include <stdint.h>

static uint8_t LED_State = 0;
void LED_Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void LED_On(void)
{
    LED_State = 1;
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void LED_Off(void)
{
    LED_State = 0;
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
}
void LED_Turn(void) {
    if (LED_State) {
        LED_State = 0;
        LED_Off();
    } else {
        LED_State = 1;
        LED_On();
    }
}