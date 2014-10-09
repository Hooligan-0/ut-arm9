
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
_irq:			.word unused_exception
_fiq:			.word unused_exception

/**
 * Dummy exception handler
 */
	.align	5
unused_exception:
	b unused_exception

.globl _bss_start
_bss_start:	.word __bss_start

.globl _bss_end
_bss_end:	.word __bss_end

.globl _end
_end:	.word __end

asm_entry:
//	mrs	r0, cpsr
//	bic	r0, r0, #0x1f
//	orr	r0, r0, #0xd3
//	msr	cpsr,r0

	ldr	r0, _stack
	sub	sp, r0, #4

	bl	loader_main	@ jump to C code
	ldr	pc, _kernel	@ jump to kernel entry

_loader_main:   .word loader_main
_kernel: .word 0x00000000
_stack:  .word 0x40007000

.section	endpad
epad:	.word 0xdeadbeef
