#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Oleksandr Rokytskyi");
MODULE_DESCRIPTION("Hello World Module 1 (Service)");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_entry {
	struct list_head list;
	ktime_t time_event;
};

static LIST_HEAD(hello_list);

void print_hello(void)
{
	struct hello_entry *entry;
	ktime_t start, end;

	start = ktime_get();

	pr_info("Hello, world!\n");

	end = ktime_get();

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		pr_err("hello1: Failed to allocate memory\n");
		return;
	}

	entry->time_event = ktime_sub(end, start);
	
	list_add_tail(&entry->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: Module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_entry *entry, *next;

	pr_info("hello1: Module exit started\n");

	list_for_each_entry_safe(entry, next, &hello_list, list) {
		pr_info("hello1: Print took %lld ns\n", ktime_to_ns(entry->time_event));
		list_del(&entry->list);
		kfree(entry);
	}
    
    pr_info("hello1: List cleaned\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
