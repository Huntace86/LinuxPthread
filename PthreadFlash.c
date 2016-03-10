#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define SIZE 1024
#define TIMER 5
#define LENTH 4

char buffer[SIZE];
pthread_mutex_t mutex;                                        //���ó�ʼ��������
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;               //�����̹߳���ͻظ�����������


void *thread_play_0(void *arg);                         



void Error_print(int num,char *pStr)                      //��װһ����⺯������Ҫ����̵߳Ľ�����һϵ�в����Ƿ���ȷ
{
	if(num != 0) {
		perror(pStr);
		exit(EXIT_FAILURE);
	}
}

int main(void)
{
	int ret;
	int i = 0;
	pthread_t id;                             //�����߳�
	pthread_attr_t thread_attr;               //�����߳�����
	
	ret = pthread_attr_init(&thread_attr);  
	Error_print(ret,"Init error\n");

	ret = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);  //���߳�����Ϊ���룬�߳̽�������Ҫ���أ����ں�̨������ͬʱִ�� 
	Error_print(ret,"Init error\n");

	ret = pthread_mutex_init(&mutex,NULL);
	Error_print(ret,"Init error\n");
	
	ret = pthread_create(&id, &thread_attr,thread_play_0,NULL);
	Error_print(ret,"Create error\n");
	
	while(scanf("%s",buffer)) {                     //���ûָ����˳�����
		if(strncmp("stop",buffer,LENTH) == 0) {      //�˳�
			pthread_mutex_lock(&mutex); 
		    printf("-------Goodbye-------\n");
			sleep(1);  
			break;
		}
		if(strncmp("start",buffer,TIMER) == 0) {      //�ָ��߳�
			pthread_mutex_lock(&mutex);
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	}

    exit(EXIT_SUCCESS);


}


void *thread_play_0(void *arg)        //�߳�ִ�к���
{
	 while (1)  
    {  
        pthread_mutex_lock(&mutex);     //ѭ����ӡ
        printf("It's a flash,input stop to stop,input pause to pause.\n");  
        pthread_mutex_unlock(&mutex); 
		sleep(1);  

		if (strncmp("stop", buffer, LENTH) == 0) {              //�˳��߳�
            break;  
        }
		if (strncmp("pause", buffer, TIMER) == 0) {             //�����߳�
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


