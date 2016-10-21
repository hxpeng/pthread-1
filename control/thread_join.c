/*DATE:2015-3-25
 *AUTHOR:WJ
 *
 *pthread_join(pthread_t tid,void **rval);
 *   调用该函数ud线程会阻塞，直到线程tid终止，而且会使tid分离
 *   如果tid已经分离，那么调用pthread_join就会失败
 *pthreaddetach（pthreadt， tid）
 *	 分离有tid制定到线程
 *	 
 */
#include "apue.h"

void *thread_fun1(void *arg)
{
	printf("wo shi thread 1\n");
	return (void *)1;
}

void *thread_fun2(void *arg)
{
	printf("wo shi thread 2\n");
	pthread_detach(pthread_self());
	pthread_exit((void *)2);
}

int main()
{
	int err1, err2;
	pthread_t tid1, tid2;
	void *rval1, *rval2;

	err1 = pthread_create(&tid1, NULL, thread_fun1, NULL);
	err2 = pthread_create(&tid2, NULL, thread_fun2, NULL);

	if(err1 || err2)
	{
		printf("create new thread failed\n");
		return 0;
	}

	printf("wo shi  main thread\n");
	printf("join1 rval is %d\n", pthread_join(tid1, &rval1));
	printf("join2 rval is %d\n", pthread_join(tid2, &rval2));

	printf("thread 1 exit code is %d\n", (int *)rval1);
	printf("thread 2 exit code is %d\n", (int *)rval2);
	printf("wo shi main thread\n");

	return 0;

}
