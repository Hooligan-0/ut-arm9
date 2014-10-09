/**
 * ARM9 Unit Test - Interrupt Collector for IMX23
 *
 * Copyright (c) 2014 Saint-Genest Gwenael <gwenael.saint-genest@agilack.fr>
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x00000000;

	. = ALIGN(4);
	.text	   :
	{
	  start.o
	  *(.text)
	}

	. = ALIGN(4);
	.rodata : { *(.rodata) }

	. = ALIGN(4);
	.data : { *(.data) }

	. = ALIGN(4);
	.got : { *(.got) }

	. = ALIGN(4);
	_end_of_copy = .; /* end_of ROM copy code here */
	__bss_start = .;
	.bss : { *(.bss) }
	__bss_end = .;

	.endpad : {
		LONG(0xaa);
	} =0xff
	__end = .;
}
