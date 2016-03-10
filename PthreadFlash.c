#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define SIZE 1024
#define TIMER 5
#define LENTH 4

char buffer[SIZE];
pthread_mutex_t mutex;                                        //设置初始化互斥量
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;               //设置线程挂起和回复的条件变量


void *thread_play_0(void *arg);                         



void Error_print(int num,char *pStr)                      //封装一个检测函数，主要检测线程的建立等一系列操作是否正确
{
	if(num != 0) {
		perror(pStr);
		exit(EXIT_FAILURE);
	}
}

int main(void)
{
	int ret;
	pthread_t id;                             //创建线程
	pthread_attr_t thread_attr;               //设置线程属性
	
	ret = pthread_attr_init(&thread_attr);  
	Error_print(ret,"Init error\n");

	ret = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);  //设线程属性为脱离，线程结束后不需要返回，能在后台与主线同时执行 
	Error_print(ret,"Init error\n");

	ret = pthread_mutex_init(&mutex,NULL);
	Error_print(ret,"Init error\n");
	
	ret = pthread_create(&id, &thread_attr,thread_play_0,NULL);
	Error_print(ret,"Create error\n");
	
	while(scanf("%s",buffer)) {                     //设置恢复和退出条件
		if(strncmp("stop",buffer,LENTH) == 0) {      //退出
			pthread_mutex_lock(&mutex); 
		        printf("-------Goodbye-------\n");
			sleep(1);  
			break;
		}
		if(strncmp("start",buffer,TIMER) == 0) {      //恢复线程
			pthread_mutex_lock(&mutex);
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	}

    exit(EXIT_SUCCESS);


}


void *thread_play_0(void *arg)        //线程执行函数
{
    while (1)  
    {  
        pthread_mutex_lock(&mutex);     //循环打印
        printf("It's a flash,input stop to stop,input pause to pause.\n");  
        pthread_mutex_unlock(&mutex); 
	sleep(1);  

	if (strncmp("stop", buffer, LENTH) == 0) {              //退出线程
            break;  
        }
	if (strncmp("pause", buffer, TIMER) == 0) {             //挂起线程
		strcpy(buffer," ");
		pthread_mutex_lock(&mutex);
		printf("---------Flash is paused---------\n");
		printf("Input start to play the flash again.\n");			
		pthread_cond_wait(&cond,&mutex);
		pthread_mutex_unlock(&mutex);
	}
    }  
    pthread_exit(NULL);
}


