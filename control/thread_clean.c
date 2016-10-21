/*DATE:			2015-4-1
 *AUTHOR:		WJ
 *DESCRIPTION:	线程清理处理程序
 *	pthread_cleanup_push（void （*rtn）（void*）， void *args）//注册处理程序
 *	pthread_cleanup_pop（int excute）//清除处理程序
 *
 *	这两个函数要成对的出现，否则编译无法通过
 *
 *	当执行以下操作时调用清理函数，清理函数的参数由args传入
 *	1、调用pthread_exit
 *	2、响应取消请求（请你来验证）
 *	3、用非零参数调用pthread_cleanup_pop
 */	

#include "apue.h"

void *fisrt_clean(void *arg)
{
	printf("%s fisrt clean\n", arg);
	return(void *)0;
}
void *second_clean(void *arg)
{
	printf("%s second clean\n", arg);
	return(void *)0;
}

void *thread_fun1(void *arg)
{
	printf("new thread 1\n");
	pthread_cleanup_push(fisrt_clean, "thread1");
	pthread_cleanup_push(second_clean, "thread1");

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);

	return(void *)1;
}
void *thread_fun2(void *arg)
{
	printf("new thread 2\n");
	pthread_cleanup_push(fisrt_clean, "thread2");
	pthread_cleanup_push(second_clean, "thread2");

	pthread_exit((void *)2);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

}

int main()
{
	pthread_t tid1, tid2;
	int err;

	err =pthread_create(&tid1, NULL, thread_fun1, NULL);
	if(err != 0)
	{
		printf("create new thread 1failed\n");
		return;
	}
	err =pthread_create(&tid2, NULL, thread_fun2, NULL);
	if(err != 0)
	{
		printf("create new thread 2failed\n");
		return;
	}

	sleep(2);

	return 0 ;
}
