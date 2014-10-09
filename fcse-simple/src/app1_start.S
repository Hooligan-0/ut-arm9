
.globl _start
_start:
	b	asm_entry

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

	bl	app1_main	@ jump to C code
ut_end:
	b	ut_end

_app1_main:   .word app1_main
_stack:  .word 0x00010000

.section	endpad
epad:	.word 0xdeadbeef
