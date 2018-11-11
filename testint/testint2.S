.data
msg: .ascii "Hello World\n"

.text
.global main

main:
    movq $1, %rax   
    movq $1, %rdi   
    movq $msg, %rsi 
    movq $12, %rdx  
    syscall         
    int $0; but get general protection exception
    movq $60, %rax  
    movq $0, %rdi   
    syscall         

