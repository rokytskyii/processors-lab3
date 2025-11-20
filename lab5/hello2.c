#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include "hello1.h" 

MODULE_AUTHOR("Oleksandr Rokytskyi");
MODULE_DESCRIPTION("Hello World Module 2 (Client)");
MODULE_LICENSE("Dual BSD/GPL");

static uint hello_count = 1;
module_param(hello_count, uint, S_IRUGO);
MODULE_PARM_DESC(hello_count, "Counter for printing Hello World");

static int __init hello2_init(void)
{
	int i;

	pr_info("hello2: Module loaded\n");

	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
		pr_warn("hello2: Warning: hello_count is %u\n", hello_count);
	} else if (hello_count > 10) {
		pr_err("hello2: Error: hello_count is too large (>10)\n");
		return -EINVAL;
	}

	for (i = 0; i < hello_count; i++) {
		print_hello();
	}

	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: Module exited\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
