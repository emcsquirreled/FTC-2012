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



/* Main Code */

blink:

/* Turn the OK LED on */
mov r1,#1
lsl r1,#16
str r1,[r0,#40]

push {r0, r1}
mov r0,#0x3F0000
wait1:
sub r0,#1
cmp r0,#0
bne wait1
pop {r0, r1}

/* Turn the OK LED off */
mov r1,#1
lsl r1,#16
str r1,[r0,#28]

push {r0, r1}
mov r0,#0x3F0000
wait2:
sub r0,#1
cmp r0,#0
bne wait2
pop {r0, r1}

/* Go back to the beginning */
b blink
