obj-m += test_dummy_irq.o
test_dummy_irq-objs := irq_dummy_test.o call_interrupt.o
# call_interrupt.o
call_interrupt.o: call_interrupt.S
#	as call_interrupt.asm -o call_interrupt.o
all:
	make -C  /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
