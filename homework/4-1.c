/*DATE:			2015-4-5
 *AUTHOR;		WJ
 *DESCRIPTION:  使用多线程对一个队列进行增加和减少，增加操作是一个线程，删除操作是一个线程
 *	
 */
#include "apue.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct queue{
	int len;
	int write_pos;
	int read_pos;
	int data[50];
};

struct queue *queue_init()
{
	struct queue *que;
	//申请内存
	que = (struct queue *)malloc(sizeof(struct queue));
	if(que ==NULL)
	{
		printf("malloc failed\n");
		return;
	}

	//初始化
	que->len = 0;
	que->write_pos = 0;
	que->read_pos = 0;

	return que;
}

void queue_destroy(struct queue *que)
{
	//销毁互斥量和que
	pthread_mutex_destroy(&mutex);
	free(que);
}

void *queue_add(void *arg)
{
	struct queue *que = (struct queue *)arg;
	int buf=0;
	while(buf<50)
	{
		pthread_mutex_lock(&mutex);
		que->data[que->write_pos] = buf;
		que->write_pos ++;
		que->len ++;
		buf++;
		printf("write data %d to queue\n", que->data[que->write_pos -1]);

		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void *queue_del(void *arg)
{
	struct queue *que = (struct queue *)arg;
	int buf=0;
	while(1)
	{
		sleep(2);
		pthread_mutex_lock(&mutex);
		buf = que->data[que->read_pos];
		que->read_pos ++;
		if(que->len -- == 0)
		{
			printf("queue is empty\n");
			return;
		}
		buf++;
		printf("read data %d from queue\n", que->data[que->read_pos -1]);
		pthread_mutex_unlock(&mutex);
	}
}

int main()
{
	pthread_t tid1, tid2;
	int err;
	struct queue *que;

	//队列和锁都要初始化
	que = queue_init();
	err = pthread_mutex_init(&mutex, NULL);
	if(err)
	{
		printf("mutex init failed\n");
		free(que);
		return;
	}

	err = pthread_create(&tid1, NULL, queue_add, (void *)que);
	if(err)
	{
		printf("create add thread failed\n");
		queue_destroy(que);
		return;
	}

	err = pthread_create(&tid2, NULL, queue_del, (void *)que);
	if(err)
	{
		printf("create del thread failed\n");
		queue_destroy(que);
		return;
	}

	//等待增加和删除操作完成
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	//销毁
	queue_destroy(que);
}
