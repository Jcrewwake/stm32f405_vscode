/* 定义程序入口点为 Reset_Handler（与启动文件一致） */
ENTRY(Reset_Handler)

/* 定义存储器区域：Flash 和 RAM 的地址与大小 */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K   /* Flash: 1 MB (STM32F405RGT6) */
  RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 192K    /* RAM: 192 KB (含128KB通用SRAM + 64KB CCMRAM需单独配置) */
}

/* 定义堆和栈的最小大小（根据应用调整） */
_Min_Heap_Size  = 0x2000;   /* 8 KB 堆（示例值） */
_Min_Stack_Size = 0x2000;   /* 8 KB 栈（示例值） */

/* 定义段分配 */
SECTIONS
{
  /* 1. 中断向量表（必须位于Flash起始） */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))   /* 保留向量表 */
    . = ALIGN(4);
  } >FLASH

  /* 2. 代码段（.text） */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* 主代码 */
    *(.text*)                /* 其他代码 */
    *(.glue_7)               /* Thumb-2 胶水代码 */
    *(.glue_7t)
    *(.eh_frame)
    KEEP(*(.init))           /* 构造函数 */
    KEEP(*(.fini))           /* 析构函数 */
    . = ALIGN(4);
    _etext = .;              /* 代码结束地址 */
  } >FLASH

  /* 3. 只读数据段（.rodata） */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)               /* 常量数据 */
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* 4. 初始化数据段（.data，加载到Flash，运行时复制到RAM） */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;              /* RAM中.data起始 */
    *(.data)                 /* 初始化数据 */
    *(.data*)
    . = ALIGN(4);
    _edata = .;             /* RAM中.data结束 */
  } >RAM AT >FLASH           /* >RAM: VMA, >FLASH: LMA */
  _sidata = LOADADDR(.data); /* Flash中.data的加载地址 */

  /* 5. 未初始化数据段（.bss） */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;               /* .bss起始 */
    *(.bss)
    *(.bss*)
    *(COMMON)                /* 公共符号 */
    . = ALIGN(4);
    _ebss = .;               /* .bss结束 */
  } >RAM

  /* 6. 用户堆空间（从.bss末尾开始） */
  . = ALIGN(4);
  .user_heap :
  {
    _end = .;                /* 堆起始 */
    . = . + _Min_Heap_Size;   /* 堆空间保留 */
    . = ALIGN(4);
    _heap_end = .;           /* 堆结束 */
  } >RAM

  /* 7. 栈空间（位于RAM末尾，向下生长） */
  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    _estack = .;              /* 栈顶地址 = RAM起始 + RAM长度 */
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  /* 8. 移除冗余段（如调试信息） */
  /DISCARD/ :
  {
    *(.ARM.exidx*)           /* 可选：若无需C++异常处理 */
    *(.comment)
  }
}