
#include "config.h"

void * memset(void * s, int c, int count);
void uart_putc(char  c);
int  uart_isready(void);
unsigned char uart_getc(void);
void uart_puts(char *s);
void uart_puthex8(unsigned char n);
void uart_puthex(unsigned long n);
void dbg_dump(void);

extern unsigned long _end;

void loader_main(void)
{
	unsigned long ttb_base;
	unsigned long vtlb;
	unsigned long i, j;
	
	uart_puts("--=={ IMX23 FCSE Unit Test }==--\r\n");

	/* Move embedded app to 32Mb aligned addr */
	for (i = 0; i < 0x1000; i +=4)
	{
	        j = *(volatile unsigned long *)(0x40009000 + i);
	        *(volatile unsigned long *)(0x42000000 + i) = j;
	}

	/* Configure TTBR (Translation Table Base register) */
	ttb_base = TLB_ADDR;
	uart_puthex(ttb_base); uart_puts("\r\n");
	asm volatile ("mcr  p15, 0, %0, c2, c0, 0" : : "r" (ttb_base) /*: */);
	uart_putc('1');

	/* First clear all TT entries (set them to Faulting) */
	memset((void *)TLB_ADDR, 0, 0x4000); // ARM_FIRST_LEVEL_PAGE_TABLE_SIZE

	ttb_base = TLB_ADDR;
	*(volatile unsigned long *)ttb_base = 0x40000C12;
	ttb_base += 4;
	for (i = 1; i < 0x1000; i++)
	{
		vtlb  = (i << 20);
		vtlb |= 0xC12;
		*(volatile unsigned long *)ttb_base = vtlb;
		ttb_base += 4;
	}

	ttb_base = TLB_ADDR;
	ttb_base += 0x80;
	*(volatile unsigned long *)ttb_base = 0x42000C12;
	uart_putc('2');

	/* Domains : Config domain0 as "client" */
	asm volatile ("mrc p15, 0, %0, c3, c0;" : "=r"(i) : /*:*/);
	i = 0x00000001;
	asm volatile ("mcr p15, 0, %0, c3, c0;" : : "r"(i) /*:*/);
	uart_putc('3');

	/* Enable MMU */
	asm volatile ("mrc p15, 0, %0, c1, c0;" : "=r"(i) : /*:*/);
	i |= (1 <<  1); /* Alignment abort enable */
	i |= (1 <<  2); /* Dcache enable */
	i |= (1 <<  0); /* MMU enable */
	i |= (1 << 11); /* Flow prediction enable */
//	i |= (1 << 13); /* Vector relocate to FFFF0000 */
	asm volatile ("mcr p15, 0, %0, c1, c0;" : : "r"(i) /*:*/);
	asm volatile ("b skip;" "nop;" "nop;" "nop;" "skip:");
	uart_putc('4');
	
	/* Set FCSE PID (=1) */
	i = 0x02000000;
	asm volatile ("mcr p15, 0, %0, c13, c0, 0;" : : "r"(i) );
	asm volatile ("nop;" "nop;");
	uart_putc('5');
	
	uart_puts("\r\n");

	return;
}

void dbg_dump(void)
{
	int i, j;
	unsigned char c;
	volatile unsigned char *pnt = (volatile unsigned char *)0x100FFF00;

	while(1)
	{
		for (i = 0; i < 16; i++)
		{
			uart_puthex((unsigned long)pnt);
			uart_puts("  ");
			for (j = 0; j < 16; j++)
			{
				uart_puthex8(*pnt);
				uart_putc(' ');
				pnt++;
			}
			uart_puts("\r\n");
		}
		while (uart_isready() == 0)
			;
		c = uart_getc();
		if (c == 'q')
			break;
		if (c == 'u')
			pnt -= 512;
	}
}

void mfill(void)
{
        unsigned long i;
        
	uart_putc('\r'); uart_putc('\n');
	for (i = 0x40000000; i < 0x42000000; i += 4)
	{
	        if ((i & 0xFFFFFF) == 0)
	        {
        	        uart_puthex(i);
        	        uart_putc('\r');
                }
	        *(volatile unsigned long *)i = i;
	}
}

/* -------------------- LIBC functions -------------------- */
void * memset(void * s, int c, int count)
{
        char *xs = (char *) s;

        while (count--)
                *xs++ = c;

        return s;
}
void *memcpy(void *dest, const void *src, int count) 
{
	char *d, *s;
        int i;
        d = (char *)dest;
	s = (char *)src;

        for (i=0 ; i < count; i++)
		d[i] = s[i];
}

/* -------------------- UART functions -------------------- */
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

int uart_isready(void)
{
        volatile unsigned long ucr2;
//        ucr2 = reg_rd(UART2_BASE + UARTx_UCR2);
        if (ucr2 & 1)
                return 1;
        return 0;
}

unsigned char uart_getc(void)
{
	return reg_rd(UART2_BASE);
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

void uart_puthex(unsigned long n)
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
