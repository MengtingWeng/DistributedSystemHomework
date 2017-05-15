#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <iostream>
#include <sched.h>
#include <sys/time.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

unsigned long counter = 0;

char *time_stamp(){
	char *timestamp = (char *)malloc(sizeof(char) * 16);
	time_t ltime;
	ltime=time(NULL);
	struct tm *tm;
	tm=localtime(&ltime);

	printf("[%04d/%02d/%02d %02d:%02d:%02d]", tm->tm_year+1900, tm->tm_mon,
    tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	return timestamp;
}

void* domain(void* agrs)
{

	cpu_set_t mask;
    CPU_ZERO(&mask);
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    pthread_mutex_lock(&mymutex);
    int target_cpu = counter % cpu_num;
    CPU_SET(target_cpu, &mask);
    pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
    counter++;
    pthread_mutex_unlock(&mymutex);

	int sockfd = 0;
	int bytesReceived = 0;
	int bufsize = 1024;
	char buffer[bufsize];
	int comNum;
	memset(buffer, '0', sizeof(buffer));
	struct sockaddr_in serv_addr;

	/* Create a socket first */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
	{
		printf("%s\n",time_stamp());
		printf("\n Error : Could not create socket \n");
		return NULL;
	}

	/* Initialize sockaddr_in data structure */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000); // port
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	/* Attempt a connection */
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	{
		printf("%s\n",time_stamp());
		printf("\n Error : Connect Failed \n");
		return NULL;
	}
        
   	if(recv(sockfd, buffer, bufsize, 0)> 0)
	{
		printf("%s\n",time_stamp());
    		printf("%s \n", buffer);
		printf("%s\n",time_stamp());
		printf("Enter # to end the connection\n");
	}

	/*begin the function of file server*/
	memset(buffer, 0, bufsize);
	do {
		printf("%s\n",time_stamp());
		printf("Client:Please choose the function...\n");
		printf("1. read\n");
		printf("2. mkdir\n");
		printf("3. rmdir\n");
		printf("4. opendir\n");
		printf("5. upload\n");
		printf("6. download\n");
		printf("Please enter the function number:\n");
		int functionNum;
		cin >> buffer;
		if(*buffer == '#') return 0;
		switch(*buffer){
			case '1':case '2':case '3':case '4':
				send(sockfd, buffer, bufsize, 0);
				printf("%s\n",time_stamp());
				printf("Enter the target file name\n");
				cin >> buffer;

				if(*buffer == '#') return 0;
				write(sockfd, buffer, 256);	// send the function number to server		
				if(read(sockfd, buffer, 256)>0)
				{
					printf("%s\n",time_stamp());
					printf("%s \n", buffer);  
				}
				break;
			case '5':
				send(sockfd, buffer, bufsize, 0);
				printf("%s\n",time_stamp());
				printf("Enter the target file name\n");
				cin >> buffer;
				write(sockfd, buffer, 256);
				if(*buffer == '#') return 0;
			        FILE *fp;
				fp  = fopen(buffer, "rb");
				if(fp == NULL)
				{
					printf("%s\n",time_stamp());
					printf(">>Fail to open this file\n");
				}
				unsigned char buff[256];
				int nread;
				nread = fread(buff, 1, 256, fp);
				write(sockfd, buff, nread);			
			        
				 if(read(sockfd, buffer, 256)>0)
				{
					printf("%s\n",time_stamp());
		 			printf("%s \n", buffer);
				}
				fclose(fp);
				break;
			case '6':
                              	send(sockfd, buffer, bufsize, 0);
                              	printf("%s\n",time_stamp());
                              	printf("Enter the target file name\n");
                              	cin >> buffer;
                              	write(sockfd, buffer, 256);
                              	if(*buffer == '#') return 0;
				
                       	fp  = fopen(buffer, "ab");
                       	if(fp == NULL)
                       	{
                               	printf("%s\n",time_stamp());
                               	printf(">>Fail to open this file\n");
                       	}

				if(read(sockfd, buffer, 256)>0)
                               {
					fwrite(buffer, 1, 256, fp);
                                       printf("%s\n",time_stamp());
					printf(">>Succcessfully download file\n");
                               }
				fclose(fp);
				break;
			default:
				break;
			
		}  

	} while(*buffer != '#');

	return NULL;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("./client thread_cnt\n");
		exit(1);
	}
	int thread_cnt = atoi(argv[1]);
	pthread_t* tids = (pthread_t*)malloc(sizeof (pthread_t) * thread_cnt);
	for (int i = 0;i < thread_cnt; ++i)
	{
		pthread_create(&tids[i], NULL, domain, NULL);
	}
	for (int i = 0;i < thread_cnt; ++i)
	{
		pthread_join(tids[i], NULL);
	}
	return 0;
}






