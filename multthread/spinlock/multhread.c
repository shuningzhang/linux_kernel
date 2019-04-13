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
#include <linux/spinlock.h>

#define BUF_SIZE 1024
struct task_struct *main_task;
struct task_struct *client_task;
spinlock_t lock;
struct task_data thread_data;


struct task_data {
	int progress;
};

static inline void sleep(unsigned sec)
{
	__set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(sec * HZ);
}

static int multhread_server(void *data)
{
	struct task_data *cur = (struct task_data *)data;
	while (!kthread_should_stop()) {
		printk(KERN_NOTICE "server thread run begin %d\n", cur->progress);
		sleep(1);
		spin_lock(&lock);
		cur->progress ++;
		spin_unlock(&lock);
		printk(KERN_NOTICE "server thread run after %d\n", cur->progress);

	}

	return 0;
}

static int multhread_client(void *data)
{
	struct task_data *cur = (struct task_data *)data;
	while(!kthread_should_stop()) {
		printk(KERN_NOTICE "client thread run begin %d\n", cur->progress);
		sleep(1);
		spin_lock(&lock);
		cur->progress += 2;
		spin_unlock(&lock);
		printk(KERN_NOTICE "client thread run after %d\n", cur->progress);

	}

	return 0;
}

static int multhread_init(void)
{
	ssize_t ret = 0;
	
	thread_data.progress = 0;
	spin_lock_init(&lock);

	printk("Hello, socket \n");
	main_task = kthread_run(multhread_server,
				  &thread_data,
				  "multhread_server");
	if (IS_ERR(main_task)) {
		ret = PTR_ERR(main_task);
		goto failed;
	}

	client_task = kthread_run(multhread_client,
				  &thread_data,
		 		  "multhread_client");
	if (IS_ERR(client_task)) {
		ret = PTR_ERR(client_task);
		goto client_failed;
	}
	
	return ret;
client_failed:
	kthread_stop(main_task);
failed:
	return ret;
}

static void multhread_exit(void)
{
	printk("Bye!\n");
	kthread_stop(main_task);
	kthread_stop(client_task);

}

module_init(multhread_init);
module_exit(multhread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

