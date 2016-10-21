/*DATE:			2015-4-17
 *AUTHOR:		DDDDD
 *DESCRIPTION:	安全到使用fork
 */
#include "apue.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_fun(void *arg)
{
	sleep(1);
	printf("new thread\n");

	pid_t pid;
	pthread_mutex_lock(&mutex);
	
	pid = fork();
	if(pid==0)
	{
		pthread_mutex_unlock(&mutex);
		printf("child\n");
	}
	if(pid>0)
	{
		pthread_mutex_unlock(&mutex);
		printf("parent\n");
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
	
	printf("main\n");
	pthread_join(tid, NULL);

	return;
}
