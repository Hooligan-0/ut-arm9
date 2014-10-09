/**
 * ARM9 Unit Test - Interrupt Collector for IMX23
 *
 * Copyright (c) 2014 Saint-Genest Gwenael <gwenael.saint-genest@agilack.fr>
 */

.globl _start
_start: b	asm_entry
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

_undefined_instruction: .word unused_exception
_software_interrupt:	.word unused_exception
_prefetch_abort:	.word unused_exception
_data_abort:		.word unused_exception
_not_used:		.word unused_exception
_irq:			.word int_handler
_fiq:			.word unused_exception

/**
 * Dummy exception handler
 */
	.align	5
unused_exception:
	b unused_exception

	.align  5
int_handler:
	push	{r0-r12}

	push	{r14}
	bl	tim_handler
	pop	{r14}

	mrs	r12, cpsr
	bic     r12, r12, #0x80  // Clear IRQ disable bit
	msr	cpsr, r12

	//
	ldr	r12, _icolllack
	mov	r0, #1
	str	r0, [r12]

	pop	{r0-r12}
	subs	pc, r14, #4
int_end:
	b	int_end

.globl _bss_start
_bss_start:	.word __bss_start

.globl _bss_end
_bss_end:	.word __bss_end

.globl _end
_end:	.word __end

asm_entry:
	// Switch to IRQ mode
	mrs	r0, cpsr
	bic	r0, r0, #0x1f  // Clear "mode" bits
	orr	r0, r0, #0xc0  // Disable IRQ and FIQ
	orr	r0, r0, #0x12  // Set IRQ mode
	msr	cpsr,r0
	// Update stack pointer for IRQ mode
	ldr	r1, _stack_irq
	sub	sp, r1, #4

	// Switch to Supervisor mode
	mrs	r0, cpsr
	bic	r0, r0, #0x1f  // Clear "mode" bits
	orr	r0, r0, #0x13  // Set Supervisor mode
	bic     r0, r0, #0x80  // Clear IRQ disable bit
	msr	cpsr,r0
	// Update stack pointer for supervisor mode
	ldr	r0, _stack_svc
	sub	sp, r0, #4

	// Call main "C" function
	bl	main
	// Infinite loop in case of return from main
asm_end:
	b	asm_end

_main:       .word main
_stack_irq:  .word 0x00007800
_stack_svc:  .word 0x00007000
_icolllack:  .word 0x80000010

_ctx_addr:	.word _save_ctx
_save_ctx:
	.word	0x00000000 // R0
	.word	0x00000000 // R1
	.word	0x00000000 // R2
	.word	0x00000000 // R3
	.word	0x00000000 // R4
	.word	0x00000000 // R5
	.word	0x00000000 // R6
	.word	0x00000000 // R7
	.word	0x00000000 // R8
	.word	0x00000000 // R9
	.word	0x00000000 // R10
	.word	0x00000000 // R11
	.word	0x00000000 // R12
	.word	0x00000000 // R13
	.word	0x00000000 // R14

.section	endpad
epad:	.word 0xdeadbeef
