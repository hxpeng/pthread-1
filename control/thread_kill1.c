#include "apue.h"

void *thread_fun(void *arg)
{
	sleep(1);
	printf("wo shi new thread\n");

	return (void *)0;
}

int main()
{
	pthread_t tid;
	int err;
	int s;
	void *rval;

	err = pthread_create(&tid, NULL, thread_fun, NULL);
	if(err != 0)
	{
		printf("create new thread failed\n");
		return;
	}
//	sleep(1);

	s = pthread_kill(tid, SIGQUIT);
	if(s == ESRCH )
	{
		printf("thread tid is not found\n");
	}

	pthread_join(tid, &rval);
	printf("wo shi main thread\n");
	return 0;

}
