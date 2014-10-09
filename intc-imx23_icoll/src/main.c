/**
 * ARM9 Unit Test - Interrupt Collector for IMX23
 *
 * Copyright (c) 2014 Saint-Genest Gwenael <gwenael.saint-genest@agilack.fr>
 */
#include "config.h"
typedef unsigned long u32;
typedef volatile unsigned long vu32;

u32  reg_rd(u32 addr);
void reg_wr(u32 addr, u32 data);
void uart_putc(char  c);
void uart_puts(char *s);
void uart_puthex8 (unsigned char n);
void uart_puthex32(unsigned long n);

extern unsigned long _end;

/**
 * Entry point for this test
 *
 */
void main(void)
{
	uart_puts("--=={ IMX23 Interrupt Collector Unit Test }==--\r\n");

	// TimRot: Set Count
	reg_wr(ADDR_TIM + 0x30, 0x0000FF00);
	
	reg_wr(ADDR_TIM + 0x20, 0x00004040);
	// TimRot: Set Select (32k)
	reg_wr(ADDR_TIM + 0x24, 0x00000008);
	// TimRot: Set Update
	reg_wr(ADDR_TIM + 0x24, 0x00000080);

	// IColl: Clear SFTRST and CLKGATE
	reg_wr(ADDR_ICOLL + 0x28, 0xC0000000);
	// IColl: Set vector 28
	reg_wr(ADDR_ICOLL + 0x2E0, 0x04);
	
	while(1)
	{
#ifdef tim_polling
		u32 v;
		v = reg_rd(ADDR_TIM + 0x20);
		if (v & 0x8000)
		{
			uart_putc('+');
			reg_wr(ADDR_TIM+0x28, 0x8000);
		}
#endif
	}
	return;
}

/**
 * Interrupt handler (by low-level asm) for Timer
 *
 */
void tim_handler(void)
{
	u32 vec;
	vec = reg_rd(ADDR_ICOLL);
	reg_wr(ADDR_TIM+0x28, 0x8000);
        uart_putc('*');
        reg_wr(ADDR_ICOLL, vec);
}

/* ----------------------- Register Mapped functions ----------------------- */
unsigned long reg_rd(unsigned long addr)
{
	unsigned long result;
	
	result = *(volatile unsigned long *)addr;
	
	return(result);
}

void reg_wr(unsigned long addr, unsigned long data)
{
	*(volatile unsigned long *)addr = data;
}

/* ---------------------------- UART  functions ---------------------------- */
#define UART2_BASE 0x80070000
#define UARTx_DR   0x00
#define UARTx_FR   0x18
void uart_putc(char c)
{
	unsigned long uart_base = UART2_BASE;

	reg_wr(uart_base + UARTx_DR, c);

	/* wait the TX_EMPTY flag */
	while(!(reg_rd(uart_base + UARTx_FR) & 0x80))
		;
}

void uart_puts(char *s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}

char hex[] = "0123456789ABCDEF";

void uart_puthex8(unsigned char n)
{
	uart_putc( hex[(n >> 4) & 0x0F] );
	uart_putc( hex[(n     ) & 0x0F] );
}

void uart_puthex32(unsigned long n)
{
	uart_putc( hex[(n >> 28) & 0x0F] );
	uart_putc( hex[(n >> 24) & 0x0F] );
	uart_putc( hex[(n >> 20) & 0x0F] );
	uart_putc( hex[(n >> 16) & 0x0F] );
	uart_putc( hex[(n >> 12) & 0x0F] );
	uart_putc( hex[(n >>  8) & 0x0F] );
	uart_putc( hex[(n >>  4) & 0x0F] );
	uart_putc( hex[(n      ) & 0x0F] );
}
/* EOF */
