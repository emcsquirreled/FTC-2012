.globl GetSystemTimerBase
GetSystemTimerBase:
	timerAddr .req r0
	ldr timerAddr,=0x20003000
	.unreq timerAddr
	mov pc,lr

.globl GetTimeStamp
GetTimeStamp:
	push {lr}
	bl GetSystemTimerBase
	ldrd r0,r1,[r0,#4]
	pop {pc}

.globl Wait
Wait:
	delay .req r2
	mov delay,r0
	push {lr}
	bl GetTimeStamp
	start .req r3
	mov start,r0

	WaitLoop1:
		bl GetTimeStamp
		elapsed .req r1
		sub elapsed,r0,start
		cmp elapsed,delay
		.unreq elapsed
		bls WaitLoop1
		
	.unreq delay
	.unreq start
	pop {pc}
