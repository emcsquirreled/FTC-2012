.section .init

.globl _start
_start:
	b Main

.section .text

Main:
	mov sp,#0x80000

	ptrn .req r4
	ldr ptrn,=pattern
	ldr ptrn,[ptrn]

	seq .req r5
	mov seq,#0

	mov r0,#16
	mov r1,#1
	bl SetGpioMode

	MainBlinkLoop:
		mov r0,#16
		mov r1,#1
		lsl r1,seq
		and r1,ptrn
		bl SetGpio

		ldr r0,=250000
		bl Wait
		
		add seq,#1
		and seq,#0b11111
	b MainBlinkLoop

.section .data

.align 2
pattern:
	.int 0b11111111101010100010001000101010
