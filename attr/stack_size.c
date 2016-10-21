/*************************************************************************
	> File Name: stack_size.c
	> Author: who
	> Created Time: 2016年03月15日 星期二 15时57分57秒
 ************************************************************************/
#include "apue.h"

pthread_attr_t attr;

void *test_fun(void *arg)
{
	int tmp_stacksize = -1;
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
	pthread_attr_getstacksize(&attr, &tmp_stacksize);
	printf("in function stacksize:%d\n", tmp_stacksize);
	pthread_attr_setstacksize(&attr, 1024);
	pthread_attr_getstacksize(&attr, &tmp_stacksize);
	printf("in function stacksize:%d\n", tmp_stacksize);
#endif
}

int main()
{
	pthread_t tid;
	int stacksize = -1, err;

	pthread_attr_init(&attr);

#ifdef _POSIX_THREAD_ATTR_STACKSIZE
	pthread_attr_getstacksize(&attr, &stacksize);
#endif
	printf("in main  stacksize is %dM\n", stacksize/1024/1024);

	err = pthread_create(&tid, &attr, test_fun, NULL);
	if (err)
	{
		printf("Create thread fail\n");
	}

	pthread_join(tid, NULL);
	pthread_attr_destroy(&attr);
	//printf("orignal stacksize is %dM\n", stacksize/1024/8);
	//pthread_attr_setstacksize(&attr, 4096);
	//pthread_attr_getstacksize(&attr, &stacksize);
	//printf("after stacksize is %d\n", stacksize/1024);
	return 0;
}
