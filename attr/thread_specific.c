/*DATE:			2015-4-17
 *AUTHOR:		WJ
 *DESCRIPTION:	线程到私有数据，	一个像errno一样到数据
 */
#include "apue.h"

pthread_key_t key;

void *thread_fun1(void *arg)
{
	printf("thread 1 start!\n");
	int a = 1;
	//将a和key关联
	pthread_setspecific(key, (void *)a);
	sleep(2);
	printf("thread 1 key->data is %d\n", pthread_getspecific(key));
}
void *thread_fun2(void *arg)
{
	sleep(1);
	printf("thread 2 start!\n");
	int a = 2;
	//将a和key关联
	pthread_setspecific(key, (void *)a);
	printf("thread 2 key->data is %d\n", pthread_getspecific(key));
}

int main()
{
	pthread_t tid1, tid2;
	
	//创造一个key
	pthread_key_create(&key, NULL);

	//创造新线程
	if(pthread_create(&tid1, NULL, thread_fun1, NULL))
	{
		printf("create new thread 1 failed\n");
		return;
	}
	if(pthread_create(&tid2, NULL, thread_fun2, NULL))
	{
		printf("create new thread 2 failed\n");
		return;
	}

	//等待新线程结束
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	pthread_key_delete(key);

	return;
}
