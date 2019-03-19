/* 这个例程用于说明等待队列的用法，在本例程中有2个线程，分别是
 * 客户端和服务端。逻辑很简单，服务线程起来的时候会等待事件发生
 * 并阻塞，客户端每隔5秒中唤醒一次服务端。*/
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
#include <linux/wait.h>

#define BUF_SIZE 1024

struct task_struct *main_task;
struct task_struct *client_task;
wait_queue_head_t wqh;

struct thread_stat
{
	int t_can_run;
};



static inline void sleep(unsigned sec)
{
	__set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(sec * HZ);
}

static int multhread_server(void *data)
{
	int index = 0;
	struct thread_stat* ts = (struct thread_stat*) data;

	while (!kthread_should_stop()) {
		printk(KERN_NOTICE "server run %d\n", index);
		index ++;
		/*在这里等待事件， 线程被阻塞在这里。 */
		wait_event(wqh, ts->t_can_run || kthread_should_stop());
		printk(KERN_NOTICE "server event over!\n");
		ts->t_can_run = 0;
	}

	printk(KERN_NOTICE "server thread end\n");
	return 0;
}

static int multhread_client(void *data)
{
	int index = 0;
	struct thread_stat* ts = (struct thread_stat*) data;

	while (!kthread_should_stop()) {
		printk(KERN_NOTICE "client run %d %d\n", 
				index,
				ts->t_can_run);
		index ++;
		sleep(2);
		if (index % 5 == 0) {
			ts->t_can_run = 1;
			wake_up(&wqh);
		}
	}

	return 0;
}



static int multhread_init(void)
{
	ssize_t ret = 0;
	struct thread_stat thread_s;
	thread_s.t_can_run = 0;



	printk("Hello, multhread \n");
	init_waitqueue_head(&wqh);
	main_task = kthread_run(multhread_server,
				&thread_s,
				"multhread_server");
	if (IS_ERR(main_task)) {
		ret = PTR_ERR(main_task);
		goto failed;
	}

	client_task = kthread_run(multhread_client,
				  &thread_s,
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

