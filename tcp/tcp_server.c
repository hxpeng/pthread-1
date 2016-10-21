/*DATA:			2015-4-20
 *AUTHOR:		WJ
 *DESCRIPTION:	多线程TCP服务器
 *	1、int socket(int domain, int type, int protocol);
 *	2、int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
 *	3、int listen(int sockfd, int backlog);
 *	4、int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
 *	5、read、write
 */	
 
#include "apue.h"

#define MAX_LISTEN 10
char buf[100];
int ad[10];
struct sockaddr_in server_ip, remote_ip;

void *thread_fun(void *arg)
{
	while(1)
	{
		printf("read data from client ; %s\n", inet_ntoa(remote_ip.sin_addr.s_addr));
		read(ad[(int)arg], buf, 100); //从链接的套接字中读取数据到buf中
		printf("buf is %s\n", buf);
	}

	return NULL;
}

int main()
{
	int server_len, remote_len;
	pthread_t tid[10];

	int err, sd;
	int i=0;


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

	
	err = bind(sd, (struct sockaddr *)(&server_ip), sizeof(struct sockaddr));
	if(err == -1)
	{
		printf("bind error, errno is %d\n", errno);
		close(sd);
		return;
	}

	
	err = listen(sd, MAX_LISTEN);
	if(err == -1)
	{
		printf("listen error , errno is %d\n", errno);
		close(sd);
		return;
	}

	remote_len = sizeof(struct sockaddr);

	while(1)
	{
		ad[i] = accept(sd, (struct sockaddr *)(&remote_ip), &remote_len);
		if(ad[i] == -1)
		{
			printf("accept error, errno is %d \n", errno);
			close(sd);
			return;
		}

		err = pthread_create(&tid[i], NULL, thread_fun, (void *)i);
		if(err != 0)
		{
			printf("create new thread failed\n");
			close(ad[i]);
		}
		i++;
	
	}
	close(sd);

	return 0;
}
