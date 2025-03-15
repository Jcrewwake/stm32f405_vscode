#ifndef __OLED_LOW_H__
#define __OLED_LOW_H__

#include "stm32f4xx.h"
#ifdef OLED_USE_I2C // 使用I2C接口  引脚接线 VCC:PA9 GND:GND SDA:PA15 SCL:PA11
#include "Delay.h"
/*引脚配置*********************/

/**
  * 函    数：OLED写SCL高低电平
  * 参    数：要写入SCL的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SCL时，此函数会被调用
  *           用户需要根据参数传入的值，将SCL置为高电平或者低电平
  *           当参数传入0时，置SCL为低电平，当参数传入1时，置SCL为高电平
  */
void OLED_W_SCL(uint8_t BitValue)
{
    /*根据BitValue的值，将SCL置高电平或者低电平*/
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)BitValue);
    
    /*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
    //...
    Delay_us(15);
    
}

/**
 * 函    数：OLED写SDA高低电平
 * 参    数：要写入SDA的电平值，范围：0/1
 * 返 回 值：无
 * 说    明：当上层函数需要写SDA时，此函数会被调用
 *           用户需要根据参数传入的值，将SDA置为高电平或者低电平
 *           当参数传入0时，置SDA为低电平，当参数传入1时，置SDA为高电平
 */
void OLED_W_SDA(uint8_t BitValue)
{
    /*根据BitValue的值，将SDA置高电平或者低电平*/
    GPIO_WriteBit(GPIOA, GPIO_Pin_15, (BitAction)BitValue);
    
    /*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
    //...
    Delay_us(15);
}

/**
 * 函    数：OLED引脚初始化
 * 参    数：无
 * 返 回 值：无
 * 说    明：当上层函数需要初始化时，此函数会被调用
 *           用户需要将SCL和SDA引脚初始化为开漏模式，并释放引脚
 */
void OLED_GPIO_Init(void)
{
    uint32_t i, j;
    
    /*在初始化前，加入适量延时，待OLED供电稳定*/
    for (i = 0; i < 1000; i ++)
    {
        for (j = 0; j < 1000; j ++);
    }
    
    /*将SCL和SDA引脚初始化为开漏模式*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*释放SCL和SDA*/
    OLED_W_SCL(1);
    OLED_W_SDA(1);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_9);
}

/*********************引脚配置*/


/*通信协议*********************/

/**
 * 函    数：I2C起始
 * 参    数：无
 * 返 回 值：无
 */
void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);		//释放SDA，确保SDA为高电平
    OLED_W_SCL(1);		//释放SCL，确保SCL为高电平
    OLED_W_SDA(0);		//在SCL高电平期间，拉低SDA，产生起始信号
    OLED_W_SCL(0);		//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
 * 函    数：I2C终止
 * 参    数：无
 * 返 回 值：无
 */
void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);		//拉低SDA，确保SDA为低电平
    OLED_W_SCL(1);		//释放SCL，使SCL呈现高电平
    OLED_W_SDA(1);		//在SCL高电平期间，释放SDA，产生终止信号
}

/**
 * 函    数：I2C发送一个字节
 * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
 * 返 回 值：无
 */
void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    
    /*循环8次，主机依次发送数据的每一位*/
    for (i = 0; i < 8; i++)
    {
        /*使用掩码的方式取出Byte的指定一位数据并写入到SDA线*/
        /*两个!的作用是，让所有非零的值变为1*/
        OLED_W_SDA(!!(Byte & (0x80 >> i)));
        OLED_W_SCL(1);	//释放SCL，从机在SCL高电平期间读取SDA
        OLED_W_SCL(0);	//拉低SCL，主机开始发送下一位数据
    }
    
    OLED_W_SCL(1);		//额外的一个时钟，不处理应答信号
    OLED_W_SCL(0);
}

/**
 * 函    数：OLED写命令
 * 参    数：Command 要写入的命令值，范围：0x00~0xFF
 * 返 回 值：无
 */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();				//I2C起始
    OLED_I2C_SendByte(0x78);		//发送OLED的I2C从机地址
    OLED_I2C_SendByte(0x00);		//控制字节，给0x00，表示即将写命令
    OLED_I2C_SendByte(Command);		//写入指定的命令
    OLED_I2C_Stop();				//I2C终止
}

/**
 * 函    数：OLED写数据
 * 参    数：Data 要写入数据的起始地址
 * 参    数：Count 要写入数据的数量
 * 返 回 值：无
 */
void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    uint8_t i;
    
    OLED_I2C_Start();				//I2C起始
    OLED_I2C_SendByte(0x78);		//发送OLED的I2C从机地址
    OLED_I2C_SendByte(0x40);		//控制字节，给0x40，表示即将写数据
    /*循环Count次，进行连续的数据写入*/
    for (i = 0; i < Count; i ++)
    {
        OLED_I2C_SendByte(Data[i]);	//依次发送Data的每一个数据
    }
    OLED_I2C_Stop();				//I2C终止
}

/*********************通信协议*/
#endif /* OLED_USE_I2C */


#ifdef OLED_USE_SPI // 使用SPI接口  引脚接线 VCC:VCC GND:GND D0:PB3 D1:PB4 RES:PB7 DC:PB6 CS:PB5
/**
 * @brief  引脚对应的SPI.
 *  GPIOB Pin3 SPI_OLED D0 -> SPI_CLOCK
 *  GPIOB Pin4 SPI_OLED D1 -> SPI_MOSI
 *  GPIOB Pin5 SPI_OLED CS -> SPI_CS
 *  GPIOB Pin6 SPI_OLED DC -> Command/Data  (0:Command, 1:Data)
 *  GPIOB Pin7 SPI_OLED RES -> Reset
 */

void OLED_SPI_W_SCK(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)value);
}
void OLED_SPI_W_MOSI(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)value);
}
void OLED_SPI_W_CS(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)value);
}
void OLED_SPI_W_DC(uint8_t value) { // Command/Data  (0:Command, 1:Data)
    GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)value);
}
void OLED_SPI_W_RES(uint8_t value) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)value);
}
void OLED_SPI_Start(void) {
    OLED_SPI_W_CS(0);
}
void OLED_SPI_Stop(void) {
    OLED_SPI_W_CS(1);
}

void OLED_SPI_Send(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        OLED_SPI_W_MOSI((data >> (7 - i)) & 0x01);
        OLED_SPI_W_SCK(1);
        OLED_SPI_W_SCK(0);
    }
}

/**
 * 函    数：OLED引脚初始化
 * 参    数：无
 * 返 回 值：无
 * 说    明：当上层函数需要初始化时，此函数会被调用
 *           用户需要将SCL和SDA引脚初始化为开漏模式，并释放引脚
 */
void OLED_GPIO_Init(void)
{
    uint32_t i, j;
    
    /*在初始化前，加入适量延时，待OLED供电稳定*/
    for (i = 0; i < 1000; i ++)
    {
        for (j = 0; j < 1000; j ++);
    }
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    OLED_SPI_W_CS(1);
    OLED_SPI_W_SCK(0);
    OLED_SPI_W_DC(1);
    OLED_SPI_W_RES(1);
}

/*********************引脚配置*/


/*通信协议*********************/
/**
 * 函    数：OLED写命令
 * 参    数：Command 要写入的命令值，范围：0x00~0xFF
 * 返 回 值：无
 */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_SPI_Start();
    OLED_SPI_W_DC(0);
    OLED_SPI_Send(Command);
    OLED_SPI_Stop();
}

/**
 * 函    数：OLED写数据
 * 参    数：Data 要写入数据的起始地址
 * 参    数：Count 要写入数据的数量
 * 返 回 值：无
 */
void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    uint8_t i;
    
    OLED_SPI_Start();
    OLED_SPI_W_DC(1);
    /*循环Count次，进行连续的数据写入*/
    for (i = 0; i < Count; i ++)
    {
        OLED_SPI_Send(Data[i]);	//依次发送Data的每一个数据
    }
    OLED_SPI_Stop();
}

/*********************通信协议*/
#endif /* OLED_USE_SPI */

#endif /* __OLED_LOW_H__ */
