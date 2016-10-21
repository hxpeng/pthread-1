/*`DATE: 2015-3-26
 *AUTHOR:WJ
 *
 *int pthread_cancle（pthread_t tid）
 *    取消tid指定的线程，成功返回0。但是取消只是发送一个请求，
 *    并不意味着等待线程终止，而且发送成功也不意味着tid一定会终止
 *int pthread_setcancelstate(int state,  int *oldstate)  
 *    设置本线程对Cancel信号的反应，state有两种值：PTHREAD_CANCEL_ENABLE（缺省）
 *    和PTHREAD_CANCEL_DISABLE，分别表示收到信号后设为CANCLED状态和忽略CANCEL信
 *    号继续运行；old_state如果不为NULL则存入原来的Cancel状态以便恢复。
 *int pthread_setcanceltype(int type, int *oldtype) 
 *	  设置本线程取消动作的执行时机，type由两种取值：PTHREAD_CANCEL_DEFFERED和
 *	  PTHREAD_CANCEL_ASYNCHRONOUS，仅当Cancel状态为Enable时有效，分别表示收到信号
 *	  后继续运行至下一个取消点再退出和立即执行取消动作（退出）；oldtype如果不为NULL
 *	  则存入运来的取消动作类型值。
 */

#include "apue.h"

void *thread_fun(void *arg)
{
	int stateval;
	int typeval;
	stateval = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if(stateval != 0)
	{
		printf("set cancel state failed\n");
	}
	printf("Im new thread\n");
	sleep(4);


	printf("about to cancel \n");
	stateval = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if(stateval != 0)
	{
		printf("set cancel state failed\n");
	}
	typeval = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(typeval != 0)
	{
		printf("set cancel type failed\n");
	}
	
	printf("first cancel point\n");
	printf("second cancel point\n");

	return (void *)20;
}

int main()
{
	pthread_t tid ;
	int err, cval, jval;
	void *rval;

	err = pthread_create(&tid, NULL, thread_fun, NULL);
	if(err != 0)
	{
		printf("create thread failed\n");
		return 0;
	}
	sleep(2);

	cval = pthread_cancel(tid);
	if(cval != 0)
	{
		printf("cancel thread failed\n");
	}
	jval = pthread_join(tid, &rval);
	
	printf("new thread exit code is %d\n", (int *)rval);

	return 0;
}
