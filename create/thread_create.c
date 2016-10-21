/*AUTHOR:	WJ
 *DATE:		2015-3-18
 *
 *
 *getpid()			获取进程ID
 *pthread_self()	获取县城ID
 *
 *int pthread_create(pthread_t *thread, 
 *					 const pthread_attr_t *attr,
 *					 void *(*start_routine) (void *), 
 *				     void *arg);
 *第一个参数，新线程id，创建成功系统回填
 *第二个参数，新线程到属性，NULL为默认属性
 *第三个参数，新线程到启动函数
 *第四个参数，传递给新线程
 */
#include "apue.h"

void print_id(char *s)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();

	//printf("%s pid is %u, tid is 0x%x\n", s, pid, tid);
	printf("%s pid is %u, tid is %lu\n", s, pid, tid);

}

void *thread_fun(void *arg)
{
	print_id(arg);
	return (void *)0;
}

int main()
{
	pthread_t ntid;
	int err;

	pid_t main_pid;
	pthread_t main_tid;

	main_pid = getpid();
	main_tid = pthread_self();
	printf("main pid: %u, main tid: %lu\n", main_pid, main_tid);

	err = pthread_create(&ntid, NULL, thread_fun, "new thread");
	if(err != 0 )
	{
		printf("create new thread failed\n");
		return 0;
	}
	
	print_id("main thread :");
	sleep(2);

	return 0;
}
