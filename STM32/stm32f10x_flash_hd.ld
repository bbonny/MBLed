/*
 * STM32F103R8T6
 */ 

MEMORY
{
	FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
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
	} > FLASH
	
	.new_firmware :
	{	      
	       _start_new_firmware = .;
	       . = . + (50 * 0x400);
	       _end_new_firmware = .;
        } > FLASH	       
	
	.data : AT (ADDR(.text) + SIZEOF(.text))
	{
		_sdata = .;
		*(.data*)
		_edata = .;
	} > RAM

	.bss :
	{
		_sstack = .;
		. = . + 512;
		_estack = .;
		_sbss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	} > RAM
}

ASSERT(SIZEOF(.new_firmware) == 0, "Attention : Place non réservé")
