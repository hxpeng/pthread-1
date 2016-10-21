#include "apue.h"

int main()
{
	char *shm = "myshm";
	char *shm1 = "myshm1";
	int shm_id, shm_id1;
	char *buf;
	pid_t pid;

	pthread_mutex_t *mutex;
	pthread_mutexattr_t mutexattr;


	//打开共享内存
	shm_id1 = shm_open(shm1, O_RDWR|O_CREAT, 0644);
	//调整共享内存大小
	ftruncate(shm_id1, 100);
	//映射共享内存,MAP_SHARED属性表明，对共享内存的任何修改都会影响其他进程
	mutex =(pthread_mutex_t *)mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, shm_id1, 0);

	pthread_mutexattr_init(&mutexattr);
#ifdef _POSIX_THREAD_PROCESS_SHARED
	pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
#endif
	pthread_mutex_init(mutex, &mutexattr);

	//打开共享内存
	shm_id = shm_open(shm, O_RDWR|O_CREAT, 0644);
	//调整共享内存大小
	ftruncate(shm_id, 100);
	//映射共享内存,MAP_SHARED属性表明，对共享内存的任何修改都会影响其他进程
	buf =(char *)mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, shm_id, 0);
	
	pid = fork();
	if(pid==0)
	{	
		//休眠1s，让父进程先运行
		sleep(1);
		printf("I'm child proccess\n");
		
		pthread_mutex_lock(mutex);
		//将共享内存内存修改为hello
		memcpy(buf, "hello", 6);
		printf("child buf is : %s\n", buf);
		pthread_mutex_unlock(mutex);
	}
	else if(pid>0)
	{
		printf("I'm parent proccess\n");

		pthread_mutex_lock(mutex);
		//修改共享内存到内容，改为world
		memcpy(buf, "world", 6);
		sleep(3);
		printf("parent buf is : %s\n", buf);
		pthread_mutex_unlock(mutex);
	}

	pthread_mutexattr_destroy(&mutexattr);
	pthread_mutex_destroy(mutex);
	//解除映射
	munmap(buf, 100);
	//消除共享内存
	shm_unlink(shm);
	//解除映射
	munmap(mutex, 100);
	//消除共享内存
	shm_unlink(shm1);
}
