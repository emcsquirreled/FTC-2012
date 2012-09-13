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
micros .req r0
mov micros,#0xFF00
bl Wait
.unreq micros

/* Turn the OK LED off */
pinNum .req r0
pinVal .req r1
mov pinNum,#16
mov pinVal,#0
bl SetGpio
.unreq pinNum
.unreq pinVal

/* Wait */
micros .req r0
mov micros,#0xFF00
bl Wait
.unreq micros

/* Go back to the beginning */
b blink
