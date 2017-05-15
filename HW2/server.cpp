#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <dirent.h>  
#include <pthread.h>
#include <sys/stat.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/types.h>

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

void* domain(void* args)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    int server = (intptr_t)args;
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    pthread_mutex_lock(&mymutex);
    int target_cpu = counter % cpu_num;
    CPU_SET(target_cpu, &mask);
    pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
    counter++;
    pthread_mutex_unlock(&mymutex);
    int bufsize = 1024;
    char buffer[bufsize];
    
    memset(buffer, '0', sizeof(buffer));

    strcpy(buffer, "Server connected...\n" );
    send(server, buffer, bufsize, 0);
    printf("%s\n",time_stamp());
    printf("Connected with client...\n");
    printf("%s\n",time_stamp());
    printf("Enter # to end the connection\n");

    FILE *fp;
    char ch;

    memset(buffer, 0, bufsize);
    do {
    recv(server, buffer, bufsize, 0);   
    if(*buffer == '1' || *buffer == '2' || *buffer == '3' || *buffer == '4'|| *buffer == '5'|| *buffer == '6' ) 
    {
        printf("%s\n",time_stamp());
        printf("Client ask for function %s \n", buffer);
     }
    switch(*buffer) {
        case '1':
            read(server, buffer, 256);
            pthread_mutex_lock(&mymutex);
            fp = fopen(buffer, "r");
            if(fp== NULL)
            {
                printf("%s\n",time_stamp());
                printf("Cannot open the file \n");
                write(server, ">>Cannot open the file \n",256);
            }
            else
            {
                unsigned char buff[256] = {0};
                int nread = fread(buff,1,256,fp);
                if(nread > 0)
                {
                  printf("%s\n",time_stamp());
                  printf("Successfully read file \n");
                  write(server, buff, nread);
                }
                fclose(fp);
            }
            pthread_mutex_unlock(&mymutex);
            break;
        case '2':
            read(server, buffer, 256);
            pthread_mutex_lock(&mymutex);
            if(mkdir(buffer, 0755)==0) 
            {           
                write(server, ">>Successfully MKDIR \n", 256);
                printf("%s\n",time_stamp());
                printf("Successfully MKDIR \n");
            }
            else
            {
                write(server, ">>Fail to MKDIR \n", 256);
                printf("%s\n",time_stamp());
                printf("Fail to MKDIR \n");
            }
            pthread_mutex_unlock(&mymutex);
            break;
        case '3':
            read(server, buffer, 256);
            pthread_mutex_lock(&mymutex);
            if(rmdir(buffer)==0) 
            {           
                write(server, ">>Successfully RMDIR \n", 256);
                printf("%s\n",time_stamp());
                printf("Successfully RMDIR \n");
            }
            else
            {
                write(server, ">>Fail to RMDIR \n", 256);
                printf("%s\n",time_stamp());
                printf("Fail to RMDIR \n");
            }
            pthread_mutex_unlock(&mymutex);
            break;
        case '4':
            DIR *dir;
            read(server, buffer, 256);
            pthread_mutex_lock(&mymutex);
            dir = opendir(buffer);
            struct dirent *ptr;
            if(dir == 0) write(server, ">>Successfully OPENDIR \n", 256);
            
            memset(buffer, 0, bufsize);
            while((ptr = readdir(dir)) != NULL )
            {
                strcat(buffer, ptr->d_name);
                strcat(buffer, "    ");
            }
                write(server, buffer, 256);         
                printf("%s\n",time_stamp());
                printf("%s\n", buffer);
            closedir(dir);
            pthread_mutex_unlock(&mymutex);
            break;
        case '5':
            read(server, buffer, 256);
            FILE *fp;
            pthread_mutex_lock(&mymutex);
            fp  = fopen(buffer, "ab");
            if(fp == NULL)
                        {
                            printf("%s\n",time_stamp());
                            printf(">>Fail to open this file\n");
                        }
                
            int bytesRecv;  
            if((bytesRecv =  read(server, buffer, 256))>0)
            {
                                fwrite(buffer, 1, bytesRecv, fp);
                write(server, ">>Succcessfully upload file\n", 256);
                fclose(fp);
                            printf("%s\n",time_stamp());
                            printf(">>Succcessfully upload file\n");
            }
            pthread_mutex_unlock(&mymutex);
            break;
        case '6':
            read(server, buffer, 256);
            pthread_mutex_lock(&mymutex);
            fp  = fopen(buffer, "rb");
            if(fp == NULL)
                        {
                            write(server, ">>Fail to open this file\n", 256);
                        }
                else {
                fread(buffer, 1, 256, fp);
                fclose(fp);
                write(server, buffer, 256); 
                printf("%s\n",time_stamp());
                                printf(">>Succcessfully download file\n");
            }
            pthread_mutex_unlock(&mymutex);
            break;

        default:
            //printf("%s\n",time_stamp());
            //printf("invalid function number \n");
            break;
              }
    
}while(*buffer != '#');
return 0;   
}

int main(void)
{
    int client = 0;
    int server = 0;
    struct sockaddr_in server_addr;
    
    //int numrv;
    socklen_t size;

	//socket(int domain, int type, int protocol)
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
	{ 
		printf("%s\n",time_stamp());
        printf("ERROR: fail to open socket\n");
	}
    else 
	{
		printf("%s\n",time_stamp());
        printf("Server socket connection created...\n"); 
 	}
    //clear address structure
    memset(&server_addr, '0', sizeof(server_addr));

	/**************setup up the host address structure************/
	//server byte order
    server_addr.sin_family = AF_INET;
	
	//automatically be filled with current host's IP address
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//convert int value of port into network byte order
    server_addr.sin_port = htons(5000);

    //bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
	//bind () passes file descriptor, the address structure and the length of the address structure.
	if(bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr)) <0) 
	{
		printf("%s\n",time_stamp());
		printf("ERROR: fail to bind\n");
        return -1;
    }
 
	//listen() call tells the socket to listen to the incoming connections.
	if(listen(client, 10) == -1)
    {
		printf("%s\n",time_stamp());
        printf("Failed to listen\n");
        return -1;
    }
    
    size = sizeof(server_addr);
    //accept() returns a new socket file descriptor for the accepted connection.
    while (1)
    {
        server = accept(client, (struct sockaddr*)&server_addr, &size);
        if(server < 0)
        {
            printf("%s\n",time_stamp());
            printf("ERROR: fail to receive the accept");
            return -1;
        }

        if(server > 0)
        {
            //CREATE THREAD
            pthread_t tid;
            pthread_create(&tid, NULL, domain, (void *)server);
            pthread_detach(tid);
        }
    }
    return 0;    
}

