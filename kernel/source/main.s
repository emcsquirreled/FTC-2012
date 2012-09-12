/* Initialize the ARM */
.section .init
.globl _start
_start:

/* Store the address for the GPIO Controller in r0 */
ldr r0,=0x20200000

/* Enable GPIO output */
mov r1,#1
lsl r1,#18
str r1,[r0,#4]

/* Turn the OK LED on */
mov r1,#1
lsl r1,#16
str r1,[r0,#40]

/* Loop forever */
loop$:
b loop$
