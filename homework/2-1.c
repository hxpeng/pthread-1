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

struct student {
	int age;
	char name[20];
	char id[4];
};

void *thread_fun(void *stu)
{
	//传入到参数是void *  需要强制转换为struct student *
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

	//第四个参数只能是void*，因此结构体需要强制转换
	err = pthread_create(&tid, NULL, thread_fun, (void *)(&stu));
	if(err != 0)
	{
		printf(" create new thread failed\n");
		return 0;
	}
	//main函数休眠1s，让新线程执行
	sleep(1);
	
	return 0;
}
