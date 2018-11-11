;global asm_call_interrupt
section .data
;asm_call_interrupt:
msg	db	"asm_call_interrupt is calling"
section .text
	global	_start
_start:
	int 0x2F
	iret
;msg	db	"asm_call_interrupt is called"
;section .data
;message: db    "asm_call_interrupt is called", 10
