#include "stm32f4xx.h"                  // Device header
#include "LED.h"
#include "Delay.h"
#include "OLED.h"

int main(void)
{
    uint16_t i = 0;
    OLED_Init();
    LED_Init();
    OLED_Printf(0, 0, OLED_6X8, "Hello World!");
    OLED_Update();
    while(1) {
        LED_Turn();
        // Delay_ms(500);
        // LED_Turn();
        // Delay_ms(500);
        i++;
        OLED_Printf(0, 16, OLED_6X8, "i:%d", i);
        OLED_Update();
    }
    return 0;
}
