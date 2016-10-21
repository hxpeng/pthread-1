/*DATA:			2015-4-5
 *AUTHOR;		WJ
 *DESCRIPTION:	将文件1.c 拷贝到11.c 12.c 13.c
 *	
 */
#include "apue.h"

char write_file[3][10] = {"11.c", "12.c", "13.c"};
char buf[1024];
int over_flag = 0;
int write_flag = 1;
int read_flag[3] = {100, 100, 100};
int src_fd, dest_fd[3],len, size;

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t read_tid, write_tid[3];

void *read_fun(void *arg)
{
	//open file
	if((src_fd = open("1.c", O_APPEND|O_RDWR)) == -1)
	{
		printf("open source file wrong\n");
		pthread_cancel(write_tid[0]);
		pthread_cancel(write_tid[1]);
		pthread_cancel(write_tid[2]);
		return NULL;
	}
	//get file length
	len = lseek(src_fd, 0, SEEK_END);
	printf("file length is %d\n", len);
	lseek(src_fd, 0, SEEK_SET);

	int num = 0;
	while(1)
	{
		if(write_flag==1)
		{
			pthread_rwlock_wrlock(&rwlock);
			over_flag = 0;
			read_flag[0] = 0;
			read_flag[1] = 1;
			read_flag[2] = 2;
			memset(buf, 0, 1024);
			num++;
			printf("read %d\n", num);
			write_flag = 2;
			size = read(src_fd, buf, 1024);
			
			//if readpos is end, read over
			if(lseek(src_fd, 0, SEEK_CUR) == len)
			{
				over_flag = 1;
				printf("read end\n");
			pthread_rwlock_unlock(&rwlock);
				break;
			}
			pthread_rwlock_unlock(&rwlock);
		}
	}
	close(src_fd);
}

void *write_fun(void *arg)
{
	//open file
	if((dest_fd[(int)arg] = open(write_file[(int)arg], O_TRUNC |O_CREAT|O_RDWR, S_IRWXU)) == -1)
	{
		printf("open destination file wrong\n");
		return NULL;
	}

	
	while(over_flag<4)
	{
		if(read_flag[(int)arg] == (int)arg)
		{
			printf("write\n");
			pthread_rwlock_rdlock(&rwlock);
			read_flag[(int)arg] = 100;
			printf("write result is %d\n",write(dest_fd[(int)arg], buf, size));
			printf("errno is %d\n", errno);
			pthread_mutex_lock(&mutex);
			over_flag++;
			write_flag++;
			if(write_flag>=5)
				write_flag = 1;
			if(over_flag>=4)
			{
				pthread_mutex_unlock(&mutex);
				pthread_rwlock_unlock(&rwlock);
				break;
			}
			pthread_mutex_unlock(&mutex);
			pthread_rwlock_unlock(&rwlock);
		}
	}
	close(dest_fd[(int)arg]);
}


int main()
{
	int err;

	err = pthread_rwlock_init(&rwlock, NULL);
	if(err)
	{
		printf("rwlock init failed\n");
		return;
	}

	err = pthread_mutex_init(&mutex, NULL);
	if(err)
	{
		printf("mutex init failed\n");
		return;
	}

	err = pthread_create(&read_tid, NULL, read_fun, NULL);
	if(err)
	{
		printf("create read thread failed\n");
		return;
	}

	int i;
	for(i=0; i<3; i++)
	{
		err = pthread_create(&write_tid[i], NULL, write_fun, (void *)i);
		if(err)
		{
			printf("create write thread failed\n");
			return;
		}
	}

	//等待增加和删除操作完成
	pthread_join(read_tid, NULL);
	for(i=0; i<3; i++)
	{
		pthread_join(write_tid[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_rwlock_destroy(&rwlock);

	return 0;
}
