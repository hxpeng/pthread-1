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
 *第二个参数，新线程到属性，NULL为默认属性(非分离)
 *第三个参数，新线程到启动函数
 *第四个参数，传递给新线程
 */
#include "apue.h"

struct student {
	int age;
	char name[20];
	char id[4];
};

void *thread_fun(void *stu)
{
	sleep(1);
	printf("student age is %d, name is %s, id is %s\n",((struct student *)stu)->age, ((struct student *)stu)->name, ((struct student *)stu)->id);
	return (void *)0;
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int err;
	int *rval;

	struct student stu;
	stu.age = 20;
	memcpy(stu.name, "zhangsan", 20);
	memcpy(stu.id, "007", 5);

	err = pthread_create(&tid, NULL, thread_fun, (void *)(&stu));
	if(err != 0)
	{
		printf(" create new thread failed\n");
		return 0;
	}


	int i;
	printf("main thread have %d args\n", argc);
	for(i=0; i<argc; i++)
	{
		printf("main thread args is %s\n", argv[i]);
	}

	pthread_exit(rval);
}
