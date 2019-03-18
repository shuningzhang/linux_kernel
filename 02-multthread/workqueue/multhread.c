#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <net/sock.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#define BUF_SIZE 1024
struct task_struct *main_task;

static inline void sleep(unsigned sec)
{
	__set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(sec * HZ);
}

static int multhread_server(void *data)
{

	while (!kthread_should_stop()) {
		printk(KERN_NOTICE "thread run\n");
		sleep(1);
	}

	return 0;
}


static int multhread_init(void)
{
	ssize_t ret = 0;

	printk("Hello, socket \n");
	main_task = kthread_run(multhread_server,
				  NULL,
				  "multhread_server");
	if (IS_ERR(main_task)) {
		ret = PTR_ERR(main_task);
		goto failed;
	}

failed:
	return ret;
}

static void multhread_exit(void)
{
	printk("Bye!\n");
	kthread_stop(main_task);

}

module_init(multhread_init);
module_exit(multhread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

