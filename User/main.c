#include "stm32f4xx.h"                  // Device header
#include "LED.h"
#include "Delay.h"
#include "OLED.h"

int main(void)
{
    OLED_Init();
    LED_Init();
    OLED_Printf(0, 0, OLED_6X8, "Hello World!: %lf", 0.918273);
    OLED_Update();
    while(1) {
        LED_Turn();
        Delay_s(4);
        LED_Turn();
        Delay_s(1);
    }
    return 0;
}
