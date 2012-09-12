/* Initialization Section */
.section .init
.globl _start
_start:
/* Branch to main */
b main

/* Text (Main) Section */
.section .text
main:
/* Move the instruction pointer to the beginning of the code */
mov sp,#0x80000

/* Set GPIO pin 16 to output */
pinNum .req r0
pinMode .req r1
mov pinNum,#16
mov pinMode,#1
bl SetGpioMode
.unreq pinNum
.unreq pinMode

blink:
/* Turn the OK LED on */
pinNum .req r0
pinVal .req r1
mov pinNum,#16
mov pinVal,#1
bl SetGpio
.unreq pinNum
.unreq pinVal

/* Wait */
push {r0, r1}
mov r0,#0x3F0000
wait1:
sub r0,#1
cmp r0,#0
bne wait1
pop {r0, r1}

/* Turn the OK LED off */
pinNum .req r0
pinVal .req r1
mov pinNum,#16
mov pinVal,#0
bl SetGpio
.unreq pinNum
.unreq pinVal

/* Wait */
push {r0, r1}
mov r0,#0x3F0000
wait2:
sub r0,#1
cmp r0,#0
bne wait2
pop {r0, r1}

/* Go back to the beginning */
b blink
