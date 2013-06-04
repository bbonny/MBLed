/*
 * STM32F103R8T6
 */

MEMORY
{
	USER_FLASH_FIRST_PAGE_ADDRESS (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
	RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
	.text :
	{
		_start = .;
		KEEP(*(.isr_vector))
		*(.text*)
		*(.rodata*)
		_etext = .;
		_sidata = .;
	} > USER_FLASH_FIRST_PAGE_ADDRESS 

	.data : AT (ADDR(.text) + SIZEOF(.text))
	{
		_sdata = .;
		*(.data*)
		_edata = .;
	} > RAM

	.bss :
	{
		_sstack = .;
		. = . + 1024;
		_estack = .;
		_sbss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	} > RAM
}


