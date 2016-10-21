/*DATE:	2015-4-6
 *AUTHOR:DDDDD
 *DESCRIPTION: 生产者与消费者问题
 */

#include "apue.h" 
  
#define BUFFER_SIZE         5       //产品库存大小  
#define PRODUCT_CNT         30      //产品生产总数   
  
struct product_cons  
{  
	int buffer[BUFFER_SIZE];  //生产产品值  
	pthread_mutex_t lock;     //互斥锁 volatile int  
    int readpos, writepos;    //读写位置  
    pthread_cond_t notempty;  //条件变量，非空  
	pthread_cond_t notfull;   //非满  
}buffer;  
  
void init(struct product_cons *p)  
{  
	pthread_mutex_init(&p->lock, NULL);     //互斥锁  
    pthread_cond_init(&p->notempty, NULL);  //条件变量  
    pthread_cond_init(&p->notfull, NULL);   //条件变量  
    p->readpos = 0;                         //读写位置  
    p->writepos = 0;  
}  
  
void finish(struct product_cons *p)  
{  
    pthread_mutex_destroy(&p->lock);     //互斥锁  
	pthread_cond_destroy(&p->notempty);  //条件变量  
	pthread_cond_destroy(&p->notfull);   //条件变量  
	p->readpos = 0;                      //读写位置  
	p->writepos = 0;  
}  
  
  
//存储 一个数据 到 bufferr  
void put(struct product_cons *p, int data) //输入产品子函数  
{
	pthread_mutex_lock(&p->lock);
	if((p->writepos+1)%BUFFER_SIZE == p->readpos)
	{
		printf("producer wait for not full\n");
		pthread_cond_wait(&p->notfull, &p->lock);
	}

	p->buffer[p->writepos] = data;
	p->writepos ++;

	if(p->writepos >= BUFFER_SIZE)
		p->writepos = 0;

	pthread_cond_signal(&p->notempty);
	pthread_mutex_unlock(&p->lock);
}  

//读，移除 一个数据 从 buffer  
int get(struct product_cons *p)  
{
	int data;

	pthread_mutex_lock(&p->lock);

	if(p->readpos == p->writepos)
	{
		printf("consumer wait for not empty\n");
		pthread_cond_wait(&p->notempty, &p->lock);
	}

	data = p->buffer[p->readpos];
	p->readpos++;

	if(p->readpos >= BUFFER_SIZE)
		p->readpos = 0;

	pthread_cond_signal(&p->notfull);

	pthread_mutex_unlock(&p->lock);

	return data;
}  

void *producer(void *data) //子线程 ，生产
{
	int n;  
	for(n = 1; n <= 50; ++n) //生产 50 个产品  
	{  
		sleep(1);  
		printf("put the %d product ...\n", n);
		put(&buffer,n);  
		printf("put the %d product success\n", n);
	}  

	printf("producer stopped\n");  

	return NULL;  
}  

void *consumer(void *data)  
{  
	static int cnt = 0;  
	int num;
	while(1)  
	{  
		sleep(2);  
		printf("get  product ...\n");  
		num = get(&buffer);
		printf("get the %d product success\n", num);  
		if(++cnt == PRODUCT_CNT)  
			break;  
	}  

	printf("consumer stopped\n");  
	return NULL;  
}  

int main(int argc, char *argv[])  
{  
	pthread_t th_a,th_b;
	void *retval;  

	init(&buffer);  

	pthread_create(&th_a, NULL, producer, 0);  
	pthread_create(&th_b, NULL, consumer, 0);  

	pthread_join(th_a, &retval);  
	pthread_join(th_b, &retval);  

	finish(&buffer);  

	return 0;  
}  
