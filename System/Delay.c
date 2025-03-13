#include "stm32f4xx.h"

/**
  * @brief  微秒级延时（适用于 STM32F405RGT6，HCLK=168MHz）
  * @param  xus 延时时长，范围：0~99863
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
    SysTick->LOAD = 168 * xus;              // 168MHz 时钟，每微秒计数168次
    SysTick->VAL = 0x00;                    // 清空当前计数值
    SysTick->CTRL = 0x00000005;             // 使用 HCLK 作为时钟源，启动定时器
    while (!(SysTick->CTRL & 0x00010000));  // 等待计数到0
    SysTick->CTRL = 0x00000004;             // 关闭定时器
}


/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
    while(xms--)
    {
        Delay_us(1000);
    }
}

/**
 * @brief  秒级延时
 * @param  xs 延时时长，范围：0~4294967295
 * @retval 无
 */
void Delay_s(uint32_t xs)
{
    while(xs--)
    {
        Delay_ms(1000);
    }
} 
