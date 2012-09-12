/* Sets r0 to the GPIO base address */
.globl GetGpioAddress
GetGpioAddress:
/* Store the address in r0 */
ldr r0=0x20200000
/* Return */
mov pc,lr

/* Sets the pin r0 to the mode r1 */
.globl SetGpioMode
SetGpioMode:
/* Validate input */
cmp r0,#53
cmpls r1,#7
movhi pc,lr
/* Save values */
push {lr}
mov r2,r0
/* Get the GPIO base address */
bl GetGpioAddress
/* Calculate address offset */
SetGpioModeLoop1:
	cmp r2,#9
	subhi r2,#10
	addhi r0,#4
	bhi SetGpioModeLoop1
/* Calculate the mode offset */
add r2, r2,lsl #1
/* Shift the mode by the offset */
lsl r1,r2
/* Store the mode at the address */
str r1,[r0]
/* Return */
pop {pc}

/* Sets the pin r0 to the value r1 */
.globl SetGpio
SetGpio:
/* Alias names to registers */
pinNum .req r0
pinVal .req r1
/* Validate input */
cmp pinNum,#53
movhi pc,lr
/* Save values */
push {lr}
mov r2,pinNum
/* Update aliases */
.unreq pinNum
pinNum .req r2
/* Get the GPIO base address */
bl GetGpioAddress
gpioAddr .req r0
/* Calculate the pin address */
pinBank .req r3
lsr pinBank,pinNum,#5
lsl pinBank,#2
add gpioAddr,pinBank
.unreq pinBank
/* Generate the bitmask for the pin */
and pinNum,#31
setBit .req r3
mov setBit,#1
lsl setBit,pinNum
.unreq pinNum
/* Check whether to set the pin high or low */
teq pinVal,#0
.unreq pinVal
streq setBit,[gpioAddr,#40]
strne setBit,[gpioAddr,#28]
.unreq setBit
.unreq gpioAddr
/* Return */
pop {pc}
