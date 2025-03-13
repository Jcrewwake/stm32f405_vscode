#include "stm32f4xx.h"                  // Device header
#include "LED.h"
#include "Delay.h"

int main(void)
{
    
    LED_Init();
    while(1) {
        LED_Turn();
        Delay_s(4);
        LED_Turn();
        Delay_s(1);
    }
    return 0;
}
