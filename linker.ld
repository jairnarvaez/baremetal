ENTRY(reset_handler)

MEMORY
{
  FLASH (xr)   : ORIGIN = 0x00000000, LENGTH = 512K
  SRAM  (rxw)  : ORIGIN = 0x20000000, LENGTH = 128K
  CODERAM (rwx) : ORIGIN = 0x00800000+124K, LENGTH = 4K
}

_stack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  _sixram = LOADADDR(.text) + SIZEOF(.text);

  .xram : AT (_sixram) {
    . = ALIGN(4);
    __xram_start = .;
    *(.xram*)
    . = ALIGN(4);
    __xram_end = .;
  } > CODERAM

  _sidata = _sixram + SIZEOF(.xram);

  .data : AT (_sidata)
  {
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > SRAM

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > SRAM
}
