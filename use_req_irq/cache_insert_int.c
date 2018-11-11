#include <linux/init.h>
#include <linux/module.h>
#include <linux/irq.h>
//#include <linux/io.h>
//#include <linux/irqdomain.h>
#include <linux/interrupt.h>
//#include <linux/of.h>
//#include <linux/of_address.h>

//#include <asm/exception.h>
//#include <asm/mach/irq.h>



//void int250_interrupt(int irq, void *dev_id, struct pt_regs *regs)
//{
//    printk("cmask_cache_insert: Interrupt should be handled there.\n");
//}



module_init(cache_insert_int_init);



module_exit(cache_insert_int_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nannan Zhao <znannan1@vt.edu>");
MODULE_DESCRIPTION("cache support for CMASK");
