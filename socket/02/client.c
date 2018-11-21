
#ifndef __KERNEL__
    #define __KERNEL__
#endif

#ifndef MODULE
    #define MODULE
#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>


static int hello_init(void)
{
	printk("Hello, itworld123.com \n");
	return 0;
}

static void hello_exit(void)
{
	printk("Bye!\n");

}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

