/*DATE：		2015-3-25
 *AUTHOR：		WJ
 *DESCRIPTION： 一个新线程自己可以取消自己
 *	int pthread_cancel（pthread_t tid）
 *	取消tid指定的线程，成功返回0。但是取消只是发送一个请求，并不意味着等待线程终止，而且发送成功也不意味着tid一定会终止
 *	int pthread_setcancelstate(int state, int *oldstate)  
 *	设置本线程对Cancel信号的反应，state有两种值：PTHREAD_CANCEL_ENABLE（缺省）和PTHREAD_CANCEL_DISABLE，
 *	int pthread_setcanceltype(int type, int *oldtype) 
 *	设置本线程取消动作的执行时机，type由两种取值：PTHREAD_CANCEL_DEFFERED和PTHREAD_CANCEL_ASYCHRONOUS，仅当Cancel状态为Enable时有效，分别表示收到信号后继续运行至下一个取消点再退出和立即执行取消动作（退出）；oldtype如果不为NULL则存入运来的取消动作类型值。
 */
#include "apue.h"

int err;
pthread_t tid;

void *thread_fun(void *arg)
{
	printf("I'm new thread\n");

	printf("I'm about to cancel myself\n");
	//设置取消类型为延时取消
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	//设置取消状态为可以取消
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	//取消自己
	err = pthread_cancel(pthread_self());
	if(err == 0)
	{
		printf("cancel myself success\n");
	}
	printf("cancel myself failed\n");
	
	pthread_exit((void *)0);
}


int main()
{
	//创造新线程
	err = pthread_create(&tid, NULL, thread_fun, NULL);
	if(err != 0)
	{
		printf("create new thread1 failed\n");
		return 0;
	}

	err = pthread_join(tid, NULL);
	if(err != 0)
	{
		printf("main thread join new thread failed\n");
	}
	else
	{
		printf("main thread join new thread success\n");
	}
	return 0;
}
