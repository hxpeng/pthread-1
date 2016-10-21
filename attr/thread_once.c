/*DATE:			2015-4-15
 *AUTHOR:		DDDDD
 *DESCRIPTION:	一次性初始化
	int pthread_once(pthread_once_t* once_control, void (*init_routine)(void));
	如果once_control为0，init_routine()就会执行
	pthread_once()成功返回之后，once_control会变为2
 */

#include "apue.h"

pthread_once_t once = 2;
pthread_t tid;

void thread_init()
{
	printf("I'm in thread 0x%x\n", tid);

}


void *thread_fun1(void *arg)
{
	tid = pthread_self();
	printf("I'm thread 0x%x\n", tid);
	printf("once is %d\n", once);
	pthread_once(&once, thread_init);
	printf("once is %d\n", once);

	return NULL;
}

void *thread_fun2(void *arg)
{
	sleep(2);
	tid = pthread_self();
	printf("I'm thread 0x%x\n", tid);
	pthread_once(&once, thread_init);

	return NULL;
}

int main()
{
	pthread_t tid1, tid2;
	int err;

	err = pthread_create(&tid1, NULL, thread_fun1, NULL);
	if(err != 0)
	{
		printf("create new thread 1 failed\n");
		return ;
	}
	err = pthread_create(&tid2, NULL, thread_fun2, NULL);
	if(err != 0)
	{
		printf("create new thread 1 failed\n");
		return ;
	}


	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
