/*DATE:			2015-4-17
 *AUTHOR:		DDDDD
 *DESCRIPTION:	安全使用fork
 */
#include "apue.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_fun(void *arg)
{
	sleep(1);
	pid_t pid;
	pid = fork();
	if(pid==0)
	{
		pthread_mutex_lock(&mutex);
		printf("child\n");
		pthread_mutex_unlock(&mutex);
	}
	if(pid>0)
	{
		pthread_mutex_lock(&mutex);
		printf("parent\n");
		pthread_mutex_unlock(&mutex);
	}
}

int main()
{
	pthread_t tid;

	if(pthread_create(&tid, NULL, thread_fun, NULL))
	{
		printf("create new thread failed\n");
		return;
	}
	
	pthread_mutex_lock(&mutex);
	sleep(2);
	pthread_mutex_unlock(&mutex);
	printf("main\n");
	pthread_join(tid, NULL);

	return;
}
