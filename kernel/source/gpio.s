.globl GetGpioAddress
GetGpioAddress:
	gpioAddr .req r0
	ldr gpioAddr,=0x20200000
	.unreq gpioAddr
	mov pc,lr

.globl SetGpioMode
SetGpioMode:
	pinNum .req r0
	pinMode .req r1
	cmp pinNum,#53
	cmpls pinMode,#7
	movhi pc,lr

	push {lr}
	mov r2,pinNum
	.unreq pinNum
	pinNum .req r2
	bl GetGpioAddress
	gpioAddr .req r0

	SetGpioModeLoop1:
		cmp pinNum,#9
		subhi pinNum,#10
		addhi gpioAddr,#4
		bhi SetGpioModeLoop1

	add pinNum, pinNum,lsl #1
	lsl pinMode,pinNum

	mask .req r3
	mov mask,#7
	lsl mask, pinNum
	.unreq pinNum

	mvn mask,mask
	oldMode .req r2
	ldr oldMode,[gpioAddr]
	and oldMode,mask
	.unreq mask

	orr pinMode,oldMode
	.unreq oldMode

	str pinMode,[gpioAddr]
	.unreq pinMode
	.unreq gpioAddr
	pop {pc}

.globl SetGpio
SetGpio:
	pinNum .req r0
	pinVal .req r1

	cmp pinNum,#53
	movhi pc,lr
	push {lr}
	mov r2,pinNum
	.unreq pinNum
	pinNum .req r2
	bl GetGpioAddress
	gpioAddr .req r0

	pinBank .req r3
	lsr pinBank,pinNum,#5
	lsl pinBank,#2
	add gpioAddr,pinBank
	.unreq pinBank

	and pinNum,#31
	setBit .req r3
	mov setBit,#1
	lsl setBit,pinNum
	.unreq pinNum

	teq pinVal,#0
	.unreq pinVal
	streq setBit,[gpioAddr,#40]
	strne setBit,[gpioAddr,#28]
	.unreq setBit
	.unreq gpioAddr
	pop {pc}
