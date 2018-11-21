#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

struct delayed_work delay_work;

static void delaywork_func(struct work_struct *work)
{
	printk("Just for test\n");
}

static int delayw_init(void)
{
	printk("module init! \n");
	INIT_DELAYED_WORK(&delay_work, delaywork_func);
	schedule_delayed_work(&delay_work, 1000);
	return 0;
}

static void delayw_exit(void)
{
	printk("Bye!\n");

}

module_init(delayw_init);
module_exit(delayw_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

