/* Sets r0 to the system timer's base address */
.globl GetSystemTimerBase
GetSystemTimerBase:
/* Load the address into r0 */
ldr r0,=0x20003000
/* Return */
mov pc,lr

/* Puts the current timer counter in r0 (low bits) and r1 (high bits */
.globl GetTimeStamp
GetTimeStamp:
/* Save the return pointer */
push {lr}
/* Get the timer address */
bl GetSystemTimerBase
/* Load the counter into r0 and r1 */
ldrd r0,r1,[r0,#4]
/* Return */
pop {pc}

/* Waits for r0 microseconds */
.globl Wait
Wait:
/* Save the requested delay */
delay .req r2
mov delay,r0
/* Save the return pointer */
push {lr}
/* Get the current timestamp */
bl GetTimeStamp
/* Record the starting timestamp */
start .req r3
mov start,r0
/* Loop until we have burned enough time */
wait_loop:
/* Get the current timestamp */
bl GetTimeStamp
/* Calculate elapsed time */
elapsed .req r1
sub elapsed,r0,start
/* Check for the end of the delay */
cmp elapsed,delay
.unreq elapsed
bls wait_loop
/* Clean up register aliases */
.unreq delay
.unreq start
/* Return */
pop {pc}
