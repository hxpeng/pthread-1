/*DATE：		2015-3-25
 *AUTHOR：		WJ
 *DESCRIPTION： 一个线程被连接成功后，另一个线程就不能再连接该线程了
 *	int pthread_join(pthead_t tid, void **rval)
调用该函数的线程会一直阻塞，直到指定的线程tid调用pthread_exit、从启动例程返回或者被取消
 */
#include "apue.h"

int err;
pthread_t tid1, tid2;

void *thread_fun1(void *arg)
{
	//睡眠1s，让线程2先执行
	sleep(1);
	printf("I'm new thread1\n");

	pthread_exit((void *)0);
}

void *thread_fun2(void *arg)
{
	printf("I'm new thread2\n");
	//连接线程1
	err = pthread_join(tid2, NULL);
	if(err != 0)
	{
		printf("new thread2 join thread1 failed\n");
	}
	else
	{
		printf("new thread2 join thread 1 success\n");
	}
	pthread_exit((void *)0);
}
int main()
{
	//创造新线程1
	err = pthread_create(&tid1, NULL, thread_fun1, NULL);
	if(err != 0)
	{
		printf("create new thread1 failed\n");
		return 0;
	}
	//创造新线程2
	err = pthread_create(&tid2, NULL, thread_fun2, NULL);
	if(err != 0)
	{
		printf("create new thread1 failed\n");
		return 0;
	}
	//连接线程1
	err = pthread_join(tid1, NULL);
	if(err != 0)
	{
		printf("main thread join thread 1 failed\n");
	}
	else
	{
		printf("main thread join thread 1 success\n");
	}
	return 0;
}
