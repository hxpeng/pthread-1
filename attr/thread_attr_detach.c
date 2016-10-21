/*DATE:			2015-4-15
 *AUTHOR:		DDDDD
 *DESCRIPTION:	设置线程到分离属性
 *如果在创建线程的时候就直到不需要了解线程的终止状态，那么可以修改pthread_attr_t结构体的detachstate属性，
 *让线程以分离状态启动。可以使用pthread_attr_setdetachstate函数来设置线程的分离状态属性。线程的分离属性有
 *两种合法值:
 *	PTHREAD_CREATE_DETACHED分离的
 *	PTHREAD_CREATE_JOINABLE 非分离的，可连接的
 *	int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
 *  int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);
 *  使用pthread_attr_getdetachstate可以获得线程的分离状态属性
 *
 *  设置线程分离属性的步骤
 *  1、定义线程属性变量pthread_attr_t attr
 *  2、初始化attr，pthread_attr_init(&attr)
 *  3、设置线程为分离或非分离 pthread_attr_setdetachstate(&attr, detachstate)
 *  4、创建线程pthread_create（&tid， &attr， thread_fun, NULL）
 *  所有的系统都会支持线程的分离状态属性
 */

#include "apue.h"


void *thread_fun1(void *arg)
{
	
	printf("I'm new thread 1\n");

	return (void *)1;
}

void *thread_fun2(void *arg)
{
	printf("I'm new thread 2\n");

	return (void *)2;
}

int main()
{
	pthread_t tid1, tid2;
	int err;

	//定义属性变量
	pthread_attr_t attr;
	//初始化属性
	pthread_attr_init(&attr);
	//设置分离状态属性，置为已分离
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	// 0 is success
	err = pthread_create(&tid1, &attr, thread_fun1, NULL);
	if(err)
	{
		printf("create new thread 1 failed\n");
		return;
	}
	
	err = pthread_create(&tid2, NULL, thread_fun2, NULL);
	if(err)
	{
		printf("create new thread 2 failed\n");
		return;
	}

	//连接线程1
	err = pthread_join(tid1, NULL);
	if(!err)
		printf("join thread 1 success\n");
	else 
		printf("join thread 1 failed\n");
	//连接线程2
	err = pthread_join(tid2, NULL);
	if(!err)
		printf("join thread 2 success\n");
	else 
		printf("join thread 2 failed\n");

	//销毁attr
	pthread_attr_destroy(&attr);

	return 0;
}
