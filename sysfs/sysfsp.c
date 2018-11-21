
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>


static int sysfsp_init(void)
{
	printk("Hello, itworld123.com \n");
	return 0;
}

static void sysfsp_exit(void)
{
	printk("Bye!\n");

}

module_init(sysfsp_init);
module_exit(sysfsp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

