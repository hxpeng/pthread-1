/*DATE：		2015-3-25
 *AUTHOR：		WJ
 *DESCRIPTION： 创建10 个线程，如果线程的参数和随机数相同，那么线程就采用exit方式退出
 */

#include "apue.h"

int num;

void *thread_fun(void *arg)
{
	printf("I'm new thread: %d\n", (int *)arg);

	//如果线程到参数arg和产生到随机数相同，那么线程就采用exit方式退出
	if(num == (int *)arg)
	{
		printf("new thread %d exit!!!\n", (int *)arg);
		exit(0);
	}
	//否则线程睡眠，让其他线程执行
	sleep(2);
	
	pthread_exit((void *)0);
}

int main()
{
	int err;
	pthread_t tid;

	//产生随机种子
	srand((unsigned int)time(NULL));
	//产生一个10以内到随机数
	num = rand()%11;

	int i=10;
	//创建10个线程，给每个线程传递参数i
	while(i--)
	{
		err = pthread_create(&tid, NULL, thread_fun, (void *)i);
		if(err != 0)
		{
			printf("create new thread failed\n");
			return 0;
		}
	}
	//睡眠1s，让新线程先运行
	sleep(1);
	return 0;
}
