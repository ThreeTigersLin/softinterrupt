section .data
	msg	db    "sm_call_interrupt is called"
section .text
	global	_start
_start:
	int 0x2F
