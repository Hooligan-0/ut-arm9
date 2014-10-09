
void uart_putc(char  c);
void uart_puts(char *s);

extern unsigned long _end;

void app1_main(void)
{
	uart_puts("==> This is App Test 1\r\n");

	while(1) ;
	
	return;
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

void uart_puts(char *s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}
/* EOF */
