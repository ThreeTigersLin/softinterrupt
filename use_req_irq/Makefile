
obj-m += cmask2.o
cmask2-objs := kcmask.o
# call_interrupt.o
#call_interrupt.o: call_interrupt.asm 
#	as call_interrupt.asm -o call_interrupt.o
all:
	make -C  /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
