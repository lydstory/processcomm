#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <inttypes.h>
#include <libgen.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <pthread.h>

#include "BaseSocks.h"
#include "SocksClient.h"
#include <iostream>
#include <string>
using namespace std;
static char progname[PATH_MAX];
static volatile char shutdowning = 0;
static volatile char blocking = 1;


static mode_t socket_mode = 0755;
/*
static void set_progname(char *argv0)
{
    size_t length = strnlen(basename(argv0), sizeof(progname) - 1);
    strncpy(progname, basename(argv0), length);
    progname[length] = '\x00';
}
*/
static int sleep_ms(unsigned int count)
{
    unsigned int seconds = (count / 1000);
    unsigned int msec = count - (seconds * 1000);
    long int nsec = msec * 1000000L;

    struct timespec duration = {
        .tv_sec = seconds,
        .tv_nsec = nsec
    };

    return nanosleep(&duration, NULL);
}

static int callback(BaseSocks basesock,int response_fd, const char *input, uint16_t nbyte)
{
	// BaseSocks basesk;
	// BaseSocks::handler_static(,&basesk);
	 //std::cout<<"my sockfd is"<<basesk.Get_SocketFd()<<std::endl;



    ssize_t result;
    std::cout<<"response_fd is "<<basesock.Get_ChannelName()<<std::endl;
    printf("responding to command: [%s]\n", input);

    if (strcmp(input, "ping") == 0) {
    //transfor
        int pid;
        pid = fork();
         if(pid < 0) {
        printf("Fork failed.\n");
        exit(1);
    } else if(pid == 0) {
        printf("Call another program /root/clientpro/pong1.\n");
        execve("/root/clientpro/pong",NULL,NULL);
    }

     result = BaseSocks::socks_server_respond2(response_fd, "pongdsdfsdfd12323", sizeof("pongdsdfsdfd12323"));
        return (int)((result < 0) ? result : 0);
    }


    if (strcmp(input, "pong") == 0) {

         result = BaseSocks::socks_server_respond2(response_fd, "pango", sizeof("pango"));
        return (int)((result < 0) ? result : 0);
    }
    if (strcmp(input, "testmain") == 0) {
 	   SocksClient clientsocks;
   	  	clientsocks.socks_client_open(2);
   	    		     	    char buf[100] = "testmaintest";
   	    		     	    clientsocks.socks_send(buf,strlen(buf));
   	    char buf2[1024];
   	    clientsocks.socks_recv(buf2,1024);


      result = BaseSocks::socks_server_respond2(response_fd, buf2, strlen(buf2));
       result = BaseSocks::socks_server_respond2(response_fd, "testmain123", strlen("testmain123"));
        return (int)((result < 0) ? result : 0);
    }
    if (strcmp(input, "testmaintest") == 0) {

         result = BaseSocks::socks_server_respond2(response_fd, "lyd", strlen("lyd"));
        return (int)((result < 0) ? result : 0);
    }

    if (strcmp(input, "empty") == 0) {
        return 0;
    }

    if (strcmp(input, "fail") == 0) {
        return -1;
    }

    if (strcmp(input, "sleep") == 0) {
        sleep_ms(5000);
        return 0;
    }

    if (strcmp(input, "set_nonblocking") == 0) {
        blocking = 0;
         result = BaseSocks::socks_server_respond2(response_fd, "ok", sizeof("ok"));
        return (int)((result < 0) ? result : 0);
    }

    if (strcmp(input, "set_blocking") == 0) {
        blocking = 1;
         result = BaseSocks::socks_server_respond2(response_fd, "ok", sizeof("ok"));
        return (int)((result < 0) ? result : 0);
    }

    if (strcmp(input, "shutdown") == 0) {
        shutdowning = 1;
        return 0;
    }

    return (int) BaseSocks::socks_server_respond2(response_fd, input, nbyte);
}

/*----------------------------------------------------------------------------*/
void *thread(void *ptr)
{
	   BaseSocks basesock;
	   basesock.run(1,callback);

   return 0;
}

void *thread2(void *ptr)
{
	 BaseSocks basesock;
	 basesock.run(2,callback);
    return 0;
}
void *thread3(void *ptr)
{
	 BaseSocks basesock;
	 basesock.run(3,callback);
    return 0;
}
void *thread4(void *ptr)
{
	 BaseSocks basesock;
	 basesock.run(4,callback);
    return 0;
}
void *thread5(void *ptr)
{
	 BaseSocks basesock;
	 basesock.run(5,callback);
    return 0;
}
void *thread6(void *ptr)
{
	 BaseSocks basesock;
	 basesock.run(6,callback);
    return 0;
}
void *thread7(void *ptr)
{
	 BaseSocks basesock;
	 basesock.run(7,callback);
    return 0;
}

int main(int argc, char **argv)
{

	pthread_t id;
	        int ret = pthread_create(&id, NULL, thread, NULL);
	        if(ret) {

	            return 1;
	        }
	        for(int i = 0;i < 3;i++) {

	            sleep(1);
	        }


	    	pthread_t id2;
	    	        int ret2 = pthread_create(&id2, NULL, thread2, NULL);
	    	        if(ret2) {

	    	            return 1;
	    	        }
	    	        for(int i = 0;i < 3;i++) {

	    	            sleep(1);
	    	        }
	    	    	pthread_t id3;
	    	    	        int ret3 = pthread_create(&id3, NULL, thread3, NULL);
	    	    	        if(ret3) {

	    	    	            return 1;
	    	    	        }
	    	    	        for(int i = 0;i < 3;i++) {

	    	    	            sleep(1);
	    	    	        }

	    	    	    	pthread_t id4;
	    	    	    	        int ret4 = pthread_create(&id4, NULL, thread4, NULL);
	    	    	    	        if(ret4) {

	    	    	    	            return 1;
	    	    	    	        }
	    	    	    	        for(int i = 0;i < 3;i++) {

	    	    	    	            sleep(1);
	    	    	    	        }

	    	    	      	    	pthread_t id5;
	    	    	    	    	    	    	        int ret5 = pthread_create(&id5, NULL, thread5, NULL);
	    	    	    	    	    	    	        if(ret5) {

	    	    	    	    	    	    	            return 1;
	    	    	    	    	    	    	        }
	    	    	    	    	    	    	        for(int i = 0;i < 3;i++) {

	    	    	    	    	    	    	            sleep(1);
	    	    	    	    	    	    	        }
	    	    	    	    	    	    	    	pthread_t id6;
	    	    	    	    	    	    		    	        int ret6= pthread_create(&id6, NULL, thread6, NULL);
	    	    	    	    	    	    		    	        if(ret6) {

	    	    	    	    	    	    		    	            return 1;
	    	    	    	    	    	    		    	        }
	    	    	    	    	    	    		    	        for(int i = 0;i < 3;i++) {

	    	    	    	    	    	    		    	            sleep(1);
	    	    	    	    	    	    		    	        }
	    	    	    	    	    	    		    	    	pthread_t id7;
	    	    	    	    	    	    		    		    	        int ret7= pthread_create(&id7, NULL, thread7, NULL);
	    	    	    	    	    	    		    		    	        if(ret7) {

	    	    	    	    	    	    		    		    	            return 1;
	    	    	    	    	    	    		    		    	        }
	    	    	    	    	    	    		    		    	        for(int i = 0;i < 3;i++) {

	    	    	    	    	    	    		    		    	            sleep(1);
	    	    	    	    	    	    		    		    	        }
	      pthread_join(id, NULL);
	      pthread_join(id2, NULL);
	      pthread_join(id3, NULL);
	      pthread_join(id4, NULL);
	      pthread_join(id5, NULL);
	      pthread_join(id6, NULL);
	      pthread_join(id7, NULL);

}
