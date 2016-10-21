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

char buf[2][100];
int ad[10];
struct sockaddr_in server_ip, remote_ip;

pthread_mutex_t mutex[2];
pthread_once_t once = PTHREAD_ONCE_INIT;

void mutex_init()
{
    pthread_mutex_init(&mutex[0], NULL);     //互斥锁  
    pthread_mutex_init(&mutex[1], NULL);     //互斥锁  
}
  
void *thread_read(void *arg)
{
	int i = (int)arg;
	pthread_once(&once, mutex_init);
	while(1)
	{
		sleep(1);
		if(i==0)
		{
			pthread_mutex_lock(&mutex[0]);
			read(ad[0], buf[0], 100);
			pthread_mutex_unlock(&mutex[0]);
		}
		else
		{
			pthread_mutex_lock(&mutex[1]);
			read(ad[1], buf[1], 100);
			pthread_mutex_unlock(&mutex[1]);
		}
	}
	return NULL;
}

void *thread_write(void *arg)
{
	int i = (int)arg;
	pthread_once(&once, mutex_init);
	while(1)
	{
		sleep(1);
		if(i==0)
		{
			pthread_mutex_lock(&mutex[1]);
			write(ad[0], buf[1], 100);
			pthread_mutex_unlock(&mutex[1]);
		}
		else
		{
			pthread_mutex_lock(&mutex[0]);
			write(ad[1], buf[0], 100);
			pthread_mutex_unlock(&mutex[0]);
		}
	}
	return NULL;
}

int main()
{
	int server_len, remote_len;
	pthread_t tid[10][2];

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

		err = pthread_create(&tid[i][0], NULL, thread_read, (void *)i);
		if(err != 0)
		{
			printf("create new thread failed\n");
			close(ad[i]);
		}

		err = pthread_create(&tid[i][1], NULL, thread_write, (void *)i);
		if(err != 0)
		{
			printf("create new thread failed\n");
			close(ad[i]);
		}
		i++;
	
	}
    pthread_mutex_destroy(&mutex[0]);     //互斥锁  
    pthread_mutex_destroy(&mutex[1]);     //互斥锁  
	close(sd);

	return 0;
}
