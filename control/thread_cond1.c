#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <time.h>  
#include <unistd.h>  
#include <pthread.h>  
  
#define BUFFER_SIZE         5       //产品库存大小  
#define PRODUCT_CNT         50      //产品生产总数   
  
//使用了循环buffer
struct product_cons  
{  
	int buffer[BUFFER_SIZE];  //生产产品值,最大容纳的值为buffer_size  
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
  
void fini(struct product_cons *p)  
{  
    pthread_mutex_destroy(&p->lock);     //互斥锁  
	pthread_cond_destroy(&p->notempty);  //条件变量  
	pthread_cond_destroy(&p->notfull);   //条件变量  
	p->readpos = 0;                      //读写位置  
	p->writepos = 0;  
}  
  
void cleanup_handler(void *arg)  
{  
	printf("cleanup_handler exec!\n");  
	pthread_mutex_t *lock = (pthread_mutex_t*)arg;  
	pthread_mutex_unlock(lock); //解锁  
}  
  
//存储 一个数据 到 bufferr, 满的时候放不进去，等待非满信号
void put(struct product_cons *p, int data) //输入产品子函数  
{  
    pthread_mutex_lock(&p->lock); //上锁  

	/*等待，直到 buffer 不为满, 位置从0开始，需要加1*/  
	while((p->writepos + 1) % BUFFER_SIZE == p->readpos) //测试空间是否已满  
	{  
		printf("producer wait for not full\n");  
		pthread_cond_wait(&p->notfull, &p->lock); //阻塞等待  
		//这里，生产者 notfull 等待消费者 pthread_cond_signal(&p->notfull);信号  
		//如果，消费者发送了 signal 信号，表示有了 空闲  
	}  

	p->buffer[p->writepos] = data; //写数据  
	p->writepos++;  
	if(p->writepos >= BUFFER_SIZE) //如果写到 尾部,返回  
		p->writepos = 0;  
	pthread_cond_signal(&p->notempty); //发送有数据信号, 写入数据之后，肯定不为空了，共享资源非空  
	pthread_mutex_unlock(&p->lock); //解锁        
}  

//读，移除 一个数据 从 buffer  
int get(struct product_cons *p)  
{  
	int data = 0;
	pthread_mutex_lock(&p->lock);
	/*等待，直到不为空, 为空时等待非空信号*/ 
	while(p->writepos == p->readpos)  
	{  
		printf("consumer wait for not empty\n");  
		pthread_cond_wait(&p->notempty,&p->lock);  
	}  
	
	/*读 一个 数据*/  
	data = p->buffer[p->readpos];          
	p->readpos++;  

	if(p->readpos >= BUFFER_SIZE) //如果读到 尾  
		p->readpos = 0;  

	pthread_cond_signal(&p->notfull);//读走数据之后，buff肯定不是满的了
	pthread_mutex_unlock(&p->lock);  

	return data;      
}  

void *producer(void *data) //子线程 ，生产
{
	int n;  
	for(n = 1; n <= 50; ++n) //生产 50 个产品  
	{  
		sleep(1);  
		printf("put the %d product\n",n);
		put(&buffer,n);  
	}  

	printf("producer stopped\n");  

	return NULL;  
}  

void *consumer(void *data)  
{  
	static int cnt = 0;  
	while(1)  
	{  
		sleep(2);  
		printf("get the %d product\n", get(&buffer));  
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

	fini(&buffer);  

	return 0;  
}  
