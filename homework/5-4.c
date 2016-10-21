/*DATE:			2015-4-17
 *AUTHOR:		WJ
 *DESCRIPTION:	线程到私有数据，	一个像errno一样到数据
 */
#include "apue.h"

pthread_key_t key;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_fun1(void *arg)
{
	char *buf;
	printf("thread 1 start!\n");
	
	//申请动态内存
	buf = malloc(6);
	if(buf != NULL)
	{
		//将buf和key关联
		pthread_setspecific(key, (void *)buf);
	}
	
	memcpy(buf, "hello", 6);
	//读取私有数据
	sleep(3);
	printf("thread 1 key->data is : %s\n", (char *)pthread_getspecific(key));

	//释放内存
	free(buf);

}

void *thread_fun2(void *arg)
{
	char *buf;
	printf("thread 2 start!\n");
	
	//申请动态内存
	buf = malloc(6);
	if(buf != NULL)
	{
		//将buf和key关联
		pthread_setspecific(key, (void *)buf);
	}
	
	memcpy(buf, "world", 6);
	//读取私有数据
	printf("thread 2 key->data is : %s\n", (char *)pthread_getspecific(key));

	//释放内存
	free(buf);
}


int main()
{
	pthread_t tid1, tid2;
	
	//创造一个key
	pthread_key_create(&key, NULL);
	pthread_mutex_init(&mutex, NULL);

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
