/*
 * LocalSocket.cpp
 *
 *  Created on: 2019年8月20日
 *      Author: root
 */

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
#include "LocalSocket.h"

#include <iostream>
#include <string>
using namespace std;
static volatile char shutdowning = 0;
static volatile char blocking = 1;
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
 int callback(BaseSocks basesock,int response_fd, const char *input, uint16_t nbyte)
{
	// BaseSocks basesk;
	// BaseSocks::handler_static(,&basesk);
	 //std::cout<<"my sockfd is"<<basesk.Get_SocketFd()<<std::endl;

    ssize_t result;
    std::cout<<"response_fd is "<<basesock.Get_ChannelName()<< "soctfd is "<<basesock.Get_SocketFd()<<std::endl;
    printf("responding to command: [%s]\n", input);
    basesock.socks_send(basesock.Get_SocketFd(),"helloworld",strlen("helloworld"));
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


       	 char buf[1024];
       	 BaseSocks::socks_recv2(basesock.Get_SocketFd(),buf,1024);
       	 std::cout<<buf<<std::endl;

       //  result = BaseSocks::socks_server_respond2(response_fd, "pango", sizeof("pango"));
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

    if (strcmp(input, "testtest") == 0) {

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
  int callback2(BaseSocks basesock,int response_fd, const char *input, uint16_t nbyte)
{
	// BaseSocks basesk;
	// BaseSocks::handler_static(,&basesk);
	 //std::cout<<"my sockfd is"<<basesk.Get_SocketFd()<<std::endl;

    ssize_t result;
    std::cout<<"response_fd is "<<basesock.Get_ChannelName()<< "soctfd is "<<basesock.Get_SocketFd()<<std::endl;
    printf("responding to command: [%s]\n", input);
    basesock.socks_send(basesock.Get_SocketFd(),"helloworld",strlen("helloworld"));
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


       	 char buf[1024];
       	 BaseSocks::socks_recv2(basesock.Get_SocketFd(),buf,1024);
       	 std::cout<<buf<<std::endl;

       //  result = BaseSocks::socks_server_respond2(response_fd, "pango", sizeof("pango"));
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

    if (strcmp(input, "testtest") == 0) {

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

LocalSocket::LocalSocket() {

}

LocalSocket::~LocalSocket() {

}
int LocalSocket::init(int socknum)
{
     if(socknum == 1)
     {
    	 servsock.run(socknum,callback);
    	 clientsock.socks_client_open(socknum);

     }
     if(socknum == 2)
     {
    	 servsock.run(socknum,callback2);
    	 clientsock.socks_client_open(socknum);

     }

     return 0;
}

