/*
 * BaseSocks.h
 *
 *  Created on: 2019年8月17日
 *      Author: root
 */

#ifndef BASESOCKS_H_
#define BASESOCKS_H_
#include <stdint.h>
#include "ProWrapper.h"
#ifndef SOCKET
#define SOCKET int
#endif
#include <string>
using namespace std;
#ifndef NULL
	#define NULL 0
#endif
class BaseSocks;
typedef int (*socks_callback_t)(BaseSocks basesock,int response_fd, const char *msg, uint16_t len);
class BaseSocks {
public:
	BaseSocks();
	BaseSocks(socks_callback_t fun):m_fun(fun){};
	virtual ~BaseSocks();
	int socks_server_close();
	int socks_server_wait();
   int socks_server_select(struct timeval *restrict timeout);
   int socks_server_poll();
   int socks_server_open(int socknum);

   int socks_server_process();
   static int socks_server_process2(BaseSocks basesock,int socket_fd, socks_callback_t callback);
   ssize_t socks_client_process(const char *filename, const char *input,
                                uint16_t nbyte, char *output, uint16_t maxlen);

   ssize_t socks_recv(void *buf, size_t bufsize);
    ssize_t socks_server_respond(int response_fd,const void *buf, uint16_t nbyte);
  //3 static
    static ssize_t socks_server_respond2(int response_fd, const void *buf, uint16_t nbyte);
    static int fd_socket_setflag2(int fd);
    static ssize_t socks_send2(int fd, const void *buf, uint16_t nbyte);
    static void serialize_uint16_2(uint16_t input, char output[2]);
    static ssize_t write_count2(int filedes, const char *buf, size_t nbyte);

  //xie
    static ssize_t read_count2(int filedes, char *buf, size_t nbyte);
    static uint16_t deserialize_uint16_2(const char input[2]);

    static int socks_process_request2(BaseSocks basesock,int socket_fd,socks_callback_t callback,
                                     uint16_t input_size);
    static int fd_socket_clearflag2(int fd);
    static int fd_socket_checkflag2(int fd);
//class static
   ssize_t read_count(int filedes, char *buf, size_t nbyte);
   void serialize_uint16(uint16_t input, char output[2]);
   uint16_t deserialize_uint16(const char input[2]);
   int socks_process_request(int socket_fd1,socks_callback_t callback,uint16_t input_size);

   ssize_t write_count(int filedes, const char *buf, size_t nbyte);
   int fd_socket_setflag(int fd);
   int fd_socket_clearflag(int fd);
   int fd_socket_checkflag(int fd);
   ssize_t socks_send(int fd,const void *buf, uint16_t nbyte);

   int run(int socknum,socks_callback_t callback);
   int run2(int socknum);
   void* handler();
    static void handler_static(void*p_A,void* pC);
    int Get_SocketFd();
    string Get_ChannelName();
private:
	int socket_fd;  //socket fd
	socks_callback_t m_fun;
	string channelname;
};

#endif /* BASESOCKS_H_ */
