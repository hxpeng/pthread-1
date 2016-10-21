/*DATA:			2015-4-20
 *AUTHOR:		WJ
 *DESCRIPTION:	多线程TC客户端 client2
 *	1、int socket(int domain, int type, int protocol);
 *	2、int connect(int sockfd, const struct sockaddr *addr,
 *	                   socklen_t addrlen);*	3、read、write
 */	
 
#include "apue.h"

char buf[100];
int sd;
struct sockaddr_in server_ip;

void *thread_write(void *arg)
{
	while(1)
	{
		write(sd, "hello", 6);
		sleep(1);
	}
}

void *thread_read(void *arg)
{
	while(1)
	{
		sleep(1);
		memset(buf, 0, 100);
		read(sd, buf, 100);
		printf("client 2 say: %s\n", buf);
	}
}

int main()
{
	int server_len, remote_len;
	pthread_t tid_read, tid_write;

	int err;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd == -1)
	{
		printf("create socket failed, errno is %d\n", errno);
		return;
	}


	server_ip.sin_family = AF_INET;
	server_ip.sin_port = htons(5678);
	server_ip.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(server_ip.sin_zero, 0, 8);

	err = connect(sd, (struct sockaddr *)(&server_ip), sizeof(struct sockaddr));
	if(err == -1)
	{
		printf("connect error\n");
		close(sd);
		return;
	}

	err = pthread_create(&tid_read, NULL, thread_read, NULL);
	if(err)
	{
		printf("create read thread failed\n");
		close(sd);
		return;
	}
	err = pthread_create(&tid_write, NULL, thread_write, NULL);
	if(err)
	{
		printf("create write thread failed\n");
		close(sd);
		return;
	}

	pthread_join(tid_read, NULL);
	pthread_join(tid_write, NULL);

	close(sd);

	return 0;
}
