/*DATE:			2015-4-17
 *AUTHOR:		DDDDD
 *DESCRIPTION:	安全的使用fork
 *	prepare  在调用fork之前会被调用
 *	parent   在fork返回父进程之前被调用
 *	child	 在fork返回子进程之前被调用
 *	可以有多个 pthread_atfork函数，这是也就会有多个处理程序（prepare，parent，child）。
 *	多个prepare的执行顺序与注册顺序相反，而parent和child的执行顺序与注册顺序相同
 *
 */
#include "apue.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void prepare1()
{
	pthread_mutex_lock(&mutex1);
	printf("I'm prepare1\n");
}

void parent1()
{
	pthread_mutex_unlock(&mutex1);
	printf("I'm parent1\n");
}

void child1()
{
	pthread_mutex_unlock(&mutex1);
	printf("I'm child1\n");
}

void prepare2()
{
	pthread_mutex_lock(&mutex2);
	printf("I'm prepare2\n");
}

void parent2()
{
	pthread_mutex_unlock(&mutex2);
	printf("I'm parent2\n");
}

void child2()
{
	pthread_mutex_unlock(&mutex2);
	printf("I'm child2\n");
}

void *thread_fun(void *arg)
{
	sleep(1);
	pid_t pid;
	pthread_atfork(prepare1, parent1, child1);
	pthread_atfork(prepare2, parent2, child2);
	pid = fork();
	if(pid==0)
	{
		pthread_mutex_lock(&mutex1);
		printf("child process\n");
		pthread_mutex_unlock(&mutex1);
	}
	if(pid>0)
	{
		pthread_mutex_lock(&mutex1);
		printf("parent process\n");
		pthread_mutex_unlock(&mutex1);
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

	pthread_mutex_lock(&mutex1);
	pthread_mutex_lock(&mutex2);
	sleep(2);
	printf("main\n");
	pthread_mutex_unlock(&mutex2);
	pthread_mutex_unlock(&mutex1);
	pthread_join(tid, NULL);
	
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);

	return;
}
