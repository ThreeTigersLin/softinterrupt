/*
 * kcmask.c - kcmask driver file
 *
 * kcmask is a backend for frontswap that takes pages that are in the process
 * of being swapped out and attempts to compress and store them in a
 * RAM-based memory pool.  This can result in a significant I/O reduction on
 * the swap device and, in the case where decompressing from RAM is faster
 * than reading from the swap device, can also improve workload performance.
 *
 * Copyright (C) 2012  Seth Jennings <sjenning@linux.vnet.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/cpu.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/atomic.h>
#include <linux/frontswap.h>
#include <linux/rbtree.h>
#include <linux/swap.h>
#include <linux/crypto.h>
#include <linux/mempool.h>
#include <linux/zpool.h>

#include <linux/mm_types.h>
#include <linux/page-flags.h>
#include <linux/swapops.h>
#include <linux/writeback.h>
#include <linux/pagemap.h>
//NANNAN
#include <linux/unistd.h>
#include <linux/mm.h>
#include <asm/unistd.h>
#include <linux/bio.h>
#include <linux/kernel_stat.h>
#include <linux/gfp.h>
#include <linux/buffer_head.h>
#include <linux/blkdev.h>
#include <linux/uio.h>
#include <asm/pgtable.h>

#include <linux/init.h>
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/interrupt.h>

//#include <asm/exception.h>
//#include <asm/mach/irq.h>

/*********************************
* statistics
**********************************/

#define RESERVED_MEMORY_OFFSET  0x100000000     /* Offset is 4GB */
unsigned long *sys_call_table = (unsigned long *)0xffffffff818001c0;

#define  IRQNO  60

extern int asm_call_interrupt(void);

static irqreturn_t cacheInsert_InterruptHandler(int IRQ_Channel, void *DeviceIdentifier)
{
	printk("cmask_cache_insert: Interrupt should be handled there.\n");
	return IRQ_HANDLED;
}

static int cache_insert_int_init(void)
{
    unsigned int irq;
    unsigned int irqflags;
    int ret;

    irq=IRQNO;
    irqflags=IRQF_NO_SUSPEND;
    printk("cmask_cache_insert: start register it.\n");

    ret = request_irq(irq, (irq_handler_t)cacheInsert_InterruptHandler,
            irqflags, "cacheinsertINT", NULL);

    if (ret!=0) {
            printk(KERN_INFO "ERROR: Cannot request IRQ %d", irq);
            printk(" - code %d , EIO %d , EINVAL %d\n", ret, EIO, EINVAL);
            return -1;
    }

    printk("CACHEINSERT_INIT\n");
    return 0;
}

static void cache_insert_int_exit(void)
{
    //unsigned int irq;
    //irq=IRQNO;
    //free_irq(irq, NULL);
    //printk("CACHEINSERT_EXIT\n");
}

/*********************************
* frontswap hooks
**********************************/

static int kcmask_frontswap_store(unsigned type, pgoff_t offset,
				struct page *page)
{
	int ret = 1;
	unsigned int dlen = PAGE_SIZE;

	pr_info("kcmask_frontswap_store!\n");
	u8 *reserved_memory, *src;
	pr_info("map reserved memory to this virtual memory space\n");
	reserved_memory = ioremap_nocache(RESERVED_MEMORY_OFFSET, dlen);

	src = kmap_atomic(page);
	pr_info("cp data to reserved memory\n");
	memcpy(reserved_memory, src, dlen);
	kunmap_atomic(src);

	sector_t sector = (sector_t)__page_file_index(page) << (PAGE_SHIFT - 9);//swap_page_sector(page);
	pr_info("sector: %ld\n", sector);

	pr_info("call asm interrupt to insert data to cache\n");
	//asm_call_interrupt();
	pr_info("finished interrupt handler\n");
	return ret;
}

static int kcmask_frontswap_load(unsigned type, pgoff_t offset,
				struct page *page)
{

	return 0; //meaning that we can successfully read from frontswap and there is no need to read from swap device.
}

/* frees an entry in kcmask */
static void kcmask_frontswap_invalidate_page(unsigned type, pgoff_t offset)
{
	return;
}

/* frees all kcmask entries for the given swap type */
static void kcmask_frontswap_invalidate_area(unsigned type)
{
	return;
}

static void kcmask_frontswap_init(unsigned type)
{
	pr_info("init front swap: Doing nothing\n");
}

/*********************************
 kcmask ops:
 Only store and load are implemented
**********************************/

static struct frontswap_ops kcmask_frontswap_ops = {
	.store = kcmask_frontswap_store,
	.load = kcmask_frontswap_load,
	.invalidate_page = kcmask_frontswap_invalidate_page,
	.invalidate_area = kcmask_frontswap_invalidate_area,
	.init = kcmask_frontswap_init
};
int irq_req_ret = 0;
/*********************************
* module init and exit
**********************************/
static int __init init_kcmask(void)
{

    int irq = 0;
    unsigned int irqflags = 0;
    int ret = 0;
    irqflags= IRQF_SHARED ;

    //irq = irq_alloc_descs(-1, 0, 1, 0);
    irq = __irq_alloc_hwirqs(1, 0, NULL);


    printk("cmask_cache_insert: start register it. irqno = %d\n", irq);

    if (irq < 0){
        return -1;
    }     

    irq_set_chip_and_handler_name(irq, &dummy_irq_chip,
				      handle_simple_irq, "kcmask");
//    irq_set_chip_and_handler(&dummy_irq_chip, );
  

    ret = request_irq(irq, (irq_handler_t)cacheInsert_InterruptHandler,
            irqflags, "cacheinsertINT", &irq_req_ret);
    irq_req_ret = ret;
    if(ret){
      printk("can not req irq. ret is %d\n", ret);
      return -1;
    }
    return 0;

}

static void __exit exit_kcmask(void)
{
	pr_info("start unregister front swap ops\n");
        if (irq_req_ret != 0){
	//     cache_insert_int_exit();
		free_irq(irq_req_ret, &irq_req_ret);
        }
	pr_info("start unregister front swap ops done\n");
	return ;
}

/* must be late so crypto has time to come up */
//late_initcall(init_kcmask);
module_init(init_kcmask);
module_exit(exit_kcmask);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nannan Zhao <znannan1@vt.edu>");
MODULE_DESCRIPTION("Kernel support for CMASK");
