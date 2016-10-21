/*************************************************************************
	> File Name: mythread.c
	> Author: who
	> Created Time: 2016年03月15日 星期二 10时37分18秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_1(void *arg)
{
	int i = 0;
	while (i <= 10)
	{
		i++;
		printf("thread_1, i:%d\n", i);
		sleep(1);
	}
}

void * thread_2(void *arg)
{
	int b = 0;
	while (b <= 10)
	{
		b++;
		printf("thread_2, b:%d\n", b);
		sleep(1);
	}
}

int main()
{
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, thread_1,NULL);
	pthread_create(&tid2, NULL, thread_2,NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
