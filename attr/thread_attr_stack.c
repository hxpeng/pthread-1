/*DATE:			2015-4-15
 *AUTHOR:		DDDDD
 *DESCRIPTION:	设置线程栈属性
 *	修改栈属性
 *	int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
 *	获取栈属性
 *	int pthread_attr_getstack(pthread_attr_t *attr, void **stackaddr, size_t *stacksize);
 *	参数stackaddr是栈的内存单元最低地址，参数stacksize是栈的大小。你要注意stackaddr并不一定是栈的开始，对于一些处理器，栈的地址是从高往低的，那么这是stackaddr是栈的结尾。
 *
 *	当然也可以单独获取或者修改栈的大小，而不去修改栈的地址。对于栈大小设置，不能小于PTHREAD_STACK_MIN
 *  int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
 *  int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);
 *  对于栈大小的设置，在创建线程之后，还可以修改。
 */

#include "apue.h"

pthread_attr_t attr;

void *thread_fun(void *arg)
{
	size_t stacksize;
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
	pthread_attr_getstacksize(&attr, &stacksize);
	printf("new thread stack size is %d\n", stacksize);
	pthread_attr_setstacksize(&attr, 1638);
	pthread_attr_getstacksize(&attr, &stacksize);
	printf("new thread stack size is %d\n", stacksize);
#endif

	
	return (void *)1;
}

int main()
{
	pthread_t tid;
	int err;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

#ifdef _POSIX_THREAD_ATTR_STACKSIZE
//	pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
#endif
	
	err = pthread_create(&tid, &attr, thread_fun, NULL);
	if(err)
	{
		printf("create new thread failed\n");
		return;
	}
	

	pthread_join(tid, NULL);
	return 0;
}
