/*DATE:2015-4-1
 *AUTHOR:WJ
 *DESCRIPTION:	多线程访问变量产生错误的例子
 */

#include "apue.h"

struct student{
	int id;
	int age;
	int name;
}stu;

//定义两个全局变量，因为两个线程都要访问
int i;
pthread_mutex_t mutex;

void *thread_fun1(void *arg)
{
	while(1)
	{
		//加锁，对整个结构体访问进行加锁，防止产生错乱
		pthread_mutex_lock(&mutex);
		stu.id = i;
		stu.age = i;
		stu.name = i;
		i++;
		if(stu.id !=stu.age || stu.id !=stu.name || stu.age!=stu.name)
		{
			printf("thread 1 %d,%d,%d\n", stu.id, stu.age, stu.name);
			break;
		}
		//访问变量完成，需要进行解锁，只有这样其他线程才能访问
		pthread_mutex_unlock(&mutex);
	}

	return(void *)0;
}

void *thread_fun2(void *arg)
{
	while(1)
	{
		//加锁，对整个结构体访问进行加锁，防止产生错乱
		pthread_mutex_lock(&mutex);
		stu.id = i;
		stu.age = i;
		stu.name = i;
		i++;
		if(stu.id!=stu.age || stu.id!=stu.name || stu.age!=stu.name)
		{
			printf("thread 2 %d,%d,%d\n", stu.id, stu.age, stu.name);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}

	return(void *)0;
}

int main()
{
	pthread_t tid1, tid2;
	int err;
	//对互斥量进行初始化，只有初始化过到互斥量才能使用
	err = pthread_mutex_init(&mutex, NULL);
	if(err != 0)
	{
		printf("init mutex failed\n");
		return;
	}
	//床在新线程
	err = pthread_create(&tid1, NULL, thread_fun1, NULL);
	if(err != 0)
	{
		printf("create new thread failed\n");
		return;
	}
	//创造新线程
	err = pthread_create(&tid2, NULL, thread_fun2, NULL);
	if(err != 0)
	{
		printf("create new thread failed\n");
		return;
	}
	//等待新线程运行结束
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
