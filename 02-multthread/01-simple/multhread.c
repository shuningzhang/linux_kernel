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

/* 这个函数用于将内核线程置于休眠状态，也就是将其调度出
 * 队列。*/
static inline void sleep(unsigned sec)
{
	__set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(sec * HZ);
}

/* 线程函数， 这个是线程执行的主体 */
static int multhread_server(void *data)
{
	int index = 0;

	/* 在线程没有被停止的情况下，循环向系统日志输出
	 * 内容， 完成后休眠1秒。*/
	while (!kthread_should_stop()) {
		printk(KERN_NOTICE "thread run %d\n", index);
		index ++;
		sleep(1);
	}

	return 0;
}


static int multhread_init(void)
{
	ssize_t ret = 0;

	printk("Hello, thread! \n");
	/* 创建并启动一个内核线程， 这里参数为线程函数，
	 * 函数的参数（NULL）,和线程名称。 */
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
	printk("Bye thread!\n");
	/* 停止线程 */
	kthread_stop(main_task);

}

module_init(multhread_init);
module_exit(multhread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

