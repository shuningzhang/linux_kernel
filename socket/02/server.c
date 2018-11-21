#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <net/sock.h>

static void listen_data_ready(struct sock *sock)
{
	printk("Just for listen ready!\n");
}

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

	sock = (struct socket*)kmalloc(sizeof(struct socket), GFP_KERNEL);
	if (NULL == sock){
		printk("malloc failed for sock!\n");
		goto out;
	}

	/*
	 * client_sock = (struct socket*)kmalloc(sizeof(struct socket), GFP_KERNEL);
	if (NULL == client_sock) {
		printk("malloc failed for sock2!\n");
		goto failed2;
	}*/

	ret = sock_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
	if (ret) {
		printk("create socket error!\n");
		goto failed1;
	}


	ret = sock_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, &client_sock);
	if (ret < 0) {
		printk("create client socket error!\n");
		goto failed1;
	}

	write_lock_bh(&sock->sk->sk_callback_lock);
	//sock->sk->sk_data_ready = listen_data_ready;
	write_unlock_bh(&sock->sk->sk_callback_lock);

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
	
	data_buffer = kmalloc(1024, GFP_KERNEL);
	if (NULL == data_buffer) {
		printk("alloc failed!\n");
		goto failed1;
	}

	memset(data_buffer, 0, 1024);


	memset(&vec, 0, sizeof(struct kvec));
	memset(&msg, 0, sizeof(struct msghdr));
	vec.iov_base = data_buffer;
	vec.iov_len = 1024;
	//msg.msg_flags = MSG_WAITALL | MSG_NOSIGNAL;

	ret = kernel_recvmsg(client_sock, &msg, &vec, 1, 1024, 0);
	printk("recv: %s\n", data_buffer);
		
	sock_release(sock);
	sock_release(client_sock);

failed1:
	kfree(client_sock);
failed2:
	kfree(sock);
out:
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

