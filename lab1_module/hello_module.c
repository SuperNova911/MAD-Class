#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int module_begin()
{
				printk(KERN_ALERT "Hello, welcome to module\n");
				return 0;
}

static void module_end()
{
				printk(KERN_ALERT "Good bye, exit module\n");
}

module_init(module_begin);
module_exit(module_end);
