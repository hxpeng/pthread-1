/*DATE:			2015-3-25
 *AUTHOR:		WJ
 *DESCRIPTION:	验证几种线程到退出方式
 */

#include "apue.h"

void *thread_fun(void *arg)
{
	//如果传到参数1，那么就采用return方式退出
	if(strcmp("1",(char *)arg)==0)
	{
		printf("new thread return!\n");
		return (void *)1;
	}
	//如果传入参数2，那么就采用pthreade_xit方式退出
	if(strcmp("2",(char *)arg)==0)
	{
		printf("new thread pthread_exit!\n");
		pthread_exit((void *)2);
	}
	//如果传入参数3，那么就采用exit方式退出
	if(strcmp("3",(char *)arg)==0)
	{
		printf("new thread exit!\n");
		exit(3);
	}
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t tid;

	err = pthread_create(&tid, NULL, thread_fun, (void *)argv[1]);
	if(err != 0)
	{
		printf("create new  thread failed\n");
		return 0;
	}

	sleep(1);
	printf("main thread\n");
	return 0;
}
