#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <net/sock.h>

#define BUF_SIZE 1024

int server_init(void)
{
	struct socket *sock;
	struct socket *client_sock;
	struct sockaddr_in s_addr;
	unsigned short port = 8000;
	int ret = 0;
	char *data_buffer = NULL;
	struct kvec vec;
	struct msghdr msg;


	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);


	ret = sock_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
	if (ret) {
		printk("create socket error!\n");
		goto out;
	}


	ret = sock_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, &client_sock);
	if (ret < 0) {
		printk("create client socket error!\n");
		goto failed2;
	}


	ret = sock->ops->bind(sock, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in));
	if (ret) {
		printk("bind socket error!\n");
		goto failed1;
	}

	ret = sock->ops->listen(sock, 10);
	if (ret) {
		printk("listen socket error!\n");
		goto failed1;
	}

	ret = sock->ops->accept(sock, client_sock, 10);
	if (ret < 0) {
		printk("accept socket error!\n");
		goto failed1;
	}
	
	data_buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
	if (NULL == data_buffer) {
		printk("alloc failed!\n");
		goto failed1;
	}

	memset(data_buffer, 0, BUF_SIZE);


	memset(&vec, 0, sizeof(struct kvec));
	memset(&msg, 0, sizeof(struct msghdr));
	vec.iov_base = data_buffer;
	vec.iov_len = BUF_SIZE;
	//msg.msg_flags = MSG_WAITALL | MSG_NOSIGNAL;

	ret = kernel_recvmsg(client_sock, &msg, &vec, 1, BUF_SIZE, 0);
	printk("recv: %s\n", data_buffer);
		
failed1:
	sock_release(client_sock);
failed2:
	sock_release(sock);
out:
	kfree(data_buffer);
	return ret;

}

static int socket_init(void)
{
	printk("Hello, socket \n");
	server_init();
	return 0;
}

static void socket_exit(void)
{
	printk("Bye!\n");

}

module_init(socket_init);
module_exit(socket_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

