/*DATE:			2015-4-5
 *AUTHOR:		DDDDD
 *DESCRIPTION:  验证可以有多个线程同时拥有读模式下到读写锁
 *	读写锁在使用之前必须初始化
 *	int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
 *	                   const pthread_rwlockattr_t *restrict attr);
 *	成功返回0 ，失败返回错误码
 *	
 *	使用完需要销毁
 *	int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
 *	成功返回0 ，失败返回错误码
 *
 *	读模式加锁       
 *	int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
 *	int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
 *	写模式加锁
 *	int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
 *	int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
 *	解锁
 *	int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
 */

#include "apue.h"

pthread_rwlock_t rwlock;
int num=0;

void *thread_fun1(void *arg)
{
	int err;

	pthread_rwlock_rdlock(&rwlock);
	printf("thread 1 print num %d\n", num);
	sleep(5);
	printf("thread 1 over\n");

	pthread_rwlock_unlock(&rwlock);
	
	return (void *)1;
}

void *thread_fun2(void *arg)
{
	int err;
	
	pthread_rwlock_wrlock(&rwlock);
	printf("thread 2 print num %d\n", num);
	sleep(5);
	printf("thread 2 is over\n");
	pthread_rwlock_unlock(&rwlock);
	
	return (void *)1;
}

int main()
{
	pthread_t tid1, tid2;
	int err;
	
	err = pthread_rwlock_init(&rwlock, NULL);
	if(err)
	{
		printf("init rwlock failed\n");
		return ;
	}

	err = pthread_create(&tid1, NULL, thread_fun1, NULL);
	if(err)
	{
		printf("create new thread 1 failed\n");
		return ;
	}
	err = pthread_create(&tid2, NULL, thread_fun2, NULL);
	if(err)
	{
		printf("create new thread 2 failed\n");
		return ;
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	pthread_rwlock_destroy(&rwlock);

	return 0;
}
