.data
msg: .ascii "Hello World\n"

.text
.global call_interrupt

call_interrupt:
    int $59
