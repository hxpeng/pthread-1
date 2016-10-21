/*DATE:			2015-3-18
 *AUTHOR:		WJ
 *DESCRIPTION:	主线程打印奇数，新线程打印偶数，交替执行
 */
#include "apue.h"

int flag = 1;

void *thread_fun(void *arg)
{
	int num = 1;
	while(1)
	{
		if(flag==0)
		{
			//将flag置1，这样只有主线程打印一次后，新线程才能打印
			flag=1;
			printf("new thread print num:  %d\n", num*2);
			num++;
		}
		//睡眠，使主线程和新线程交替执行
		sleep(1);
	}
}

int main()
{
	int err, num=0;
	pthread_t tid;

	err = pthread_create(&tid, NULL, thread_fun, NULL);
	if(err != 0)
	{
		printf("create new thread failed \n");
		return 0;
	}
	
	while(num<20)
	{
		//flag==1主线程打印
		if(flag==1)
		{
			//将flag置0，这样只有新线程打印一次，主线程才能打印
			flag = 0;
			printf("main thread print num: %d\n", num*2+1);
			num++;
		}
		//睡眠，使得主线程和新线程可以交替执行
		sleep(1);
	}

	return 0;
}
