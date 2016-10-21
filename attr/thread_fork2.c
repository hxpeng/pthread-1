/*DATE:			2015-4-17
 *AUTHOR:		DDDDD
 *DESCRIPTION:	安全的使用fork
 *	prepare  在调用fork之前会被调用
 *	parent   在fork返回父进程之前被调用
 *	child	 在fork返回子进程之前被调用
 */
#include "apue.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void prepare()
{
	pthread_mutex_lock(&mutex);
	printf("I'm prepare\n");
}


void parent()
{
	pthread_mutex_unlock(&mutex);
	printf("I'm parent\n");
}
void child()
{
	pthread_mutex_unlock(&mutex);
	printf("I'm child\n");
}

void *thread_fun(void *arg)
{
	sleep(1);
	pid_t pid;
	pthread_atfork(prepare, parent, child);
	pid = fork();
	if(pid==0)
	{
		pthread_mutex_lock(&mutex);
		printf("child process\n");
		pthread_mutex_unlock(&mutex);
	}
	if(pid>0)
	{
		pthread_mutex_lock(&mutex);
		printf("parent process\n");
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
	printf("main\n");
	pthread_mutex_unlock(&mutex);
	pthread_join(tid, NULL);
	
	pthread_mutex_destroy(&mutex);

	return;
}
