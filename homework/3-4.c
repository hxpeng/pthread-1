/*DATE:			2015-3-31
 *AUTHOR:		WJ
 *DECRIPTION:	正确到处理信号

 *	int pthread_kill(pthread_t thread, int sig);
 *		向线程thread发送sig信号，成功返回0，失败返回错误码
 *
 *	int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
 *		为信号signum设置处理函数，处理函数在sigaction中指定
 *		act.sa_mask 信号屏蔽字
 *		act.sa_handler 信号集处理程序
 *
 *  int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
 *		多线程信号屏蔽函数
 *		how = SIG_BLOCK：向当前的信号掩码中添加set，其中set表示要阻塞的信号组。
 *		SIG_UNBLOCK：向当前的信号掩码中删除set，其中set表示要取消阻塞的信号组。
 *      SIG_SETMASK：将当前的信号掩码替换为set，其中set表示新的信号掩码。
 *      在多线程中，新线程的当前信号掩码会继承创造它的线程的信号掩码
 */
#include "apue.h"

int i=0;
//将信号放在数组
int sig[3] = {SIGQUIT, SIGKILL, SIGSTOP};
void (*handler[3])(int);
//函数指针数组
void *(*fun[3])(void *);

void sig_handler1(int arg)
{
	printf("thread1 get signal SIGQUIT\n");
	return;
}
void sig_handler2(int arg)
{
	printf("thread2 get signal SIGKILL\n");
	return;
}

void sig_handler3(int arg)
{
	printf("thread3 get signal SIGSTOP\n");
	return;
}

void *thread_fun1(void *args)
{
	int arg = (int)args;
	printf("new thread %d\n", arg);

	sigset_t set;
	sigaddset(&set, SIGQUIT);
//	pthread_sigmask(SIG_BLOCK, &set, NULL);
	
	printf("new thread over %d\n", arg);
	sleep(2);
}

void *thread_fun2(void *args)
{
	int arg = (int)args;
	printf("new thread %d\n", arg);

	sigset_t set;
	sigaddset(&set, sig[arg]);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	
	printf("new thread over %d\n", arg);
	sleep(2);
}

void *thread_fun3(void *args)
{
	int arg = (int)args;
	printf("new thread %d\n", arg);

	sigset_t set;
	sigaddset(&set, sig[arg]);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	
	printf("new thread over %d\n", arg);
	sleep(2);
}

int main()	
{
	pthread_t tid[3];
	int err;
	int s;
	struct sigaction act[3];

	//将处理函数放在数组
	handler[0] = sig_handler1;
	handler[1] = sig_handler2;
	handler[2] = sig_handler3;

	//将新线程启动例程放在数组
	fun[0] = thread_fun1;
	fun[1] = thread_fun2;
	fun[2] = thread_fun3;

	for(i=0; i<3; i++)
	{
		memset(&act[i], 0, sizeof(act[i]));
		sigaddset(&act[i].sa_mask, sig[i]);
		act[i].sa_handler = handler[i];

		//捕捉信号，设置处理函数
		err = sigaction(sig[i], &act[i], NULL);
		if(err != 0)
		{
			//试图修改SIGKILL和SIGSTOP的处理程序会造成失败
			printf("errno is %d\n", err);
			if(err == EFAULT)
				printf("memmory error\n");
			if(err == EINVAL)
				printf("invalid signal\n");
		}

		//创造新线程
		err = pthread_create(&tid[i], NULL, fun[i], (void *)i);
		if(err != 0)
		{
			printf("create new thread %d failed\n", i);
			return;
		}
	}

	//休眠1s，让新线程去设置自己到信号掩码
	sleep(1);

	for(i=0; i<3; i++)
	{
		//向线程发送信号
		s = pthread_kill(tid[i], sig[i]);
		if(s != 0)
			printf("send signal to thread %d failed\n", i);
		else
			printf("send signal to thread %d success\n", i);

		//等待新线程结束
		pthread_join(tid[i], NULL);
	}


	return 0;
}
