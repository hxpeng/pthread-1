#include "apue.h"

void *thread_fun(void *arg)
{
	printf("wo\n");

	return (void *)0;
}

void handler(int arg)
{
	printf("hand\n");
}
int main()
{
	pthread_t tid;
	void *rval;
	struct sigaction act;

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	sigfillset(&act.sa_mask);
//	sigaction(SIGQUIT, &act, NULL);

	pthread_create(&tid, NULL, thread_fun, NULL);
	pthread_kill(tid, SIGQUIT);
	pthread_join(tid, &rval);
}

