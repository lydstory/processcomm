/*
 * BaseSocks.cpp
 *
 *  Created on: 2019年8月17日
 *      Author: root
 */

#include "BaseSocks.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include "Tqtype.h"
static volatile char shutdowning = 0;
static volatile char blocking = 1;


static mode_t socket_mode = 0755;

#include <iostream>
#include <string>
using namespace std;
#define get_size(type, field) sizeof(((type *)0)->field)
enum {
    sun_path_size = get_size(struct sockaddr_un, sun_path) - 1,
    address_maxlen = (sun_path_size > PATH_MAX) ? sun_path_size : PATH_MAX,
    backlog_target = 16,
    backlog_size = (backlog_target < SOMAXCONN) ? backlog_target : SOMAXCONN
};

void BaseSocks::serialize_uint16(uint16_t input, char output[2])
{
    output[0] = (char)((input >> 0) & 0xFF);
    output[1] = (char)((input >> 8) & 0xFF);
}
void BaseSocks::serialize_uint16_2(uint16_t input, char output[2])
{
    output[0] = (char)((input >> 0) & 0xFF);
    output[1] = (char)((input >> 8) & 0xFF);
}

/** @brief Deserializes a little-endian 2-char array and returns the result.
 * Used to ensure predictable deserialization across platforms.
 * @param[in] input Serialized data
 * @return Deserialized uint16 */
uint16_t BaseSocks::deserialize_uint16(const char input[2])
{
    int result = 0;

    result += (input[0]) << 0;
    result += (input[1]) << 8;

    return (uint16_t) (result & 0xFFFF);
}
uint16_t BaseSocks::deserialize_uint16_2(const char input[2])
{
    int result = 0;

    result += (input[0]) << 0;
    result += (input[1]) << 8;

    return (uint16_t) (result & 0xFFFF);
}
ssize_t BaseSocks::write_count2(int filedes, const char *buf, size_t nbyte)
{
    size_t remaining = nbyte;

    while (remaining != 0) {
        ssize_t result = Pro_Wrapper::write_noeintr(filedes, buf, remaining);

        if (result < 0) {
            return result;
        }

        remaining -= (size_t) result;
        buf += result;
    }

    return (ssize_t) nbyte;
}
ssize_t BaseSocks::read_count2(int filedes, char *buf, size_t nbyte)
{
    size_t total = 0;

    while (total < nbyte) {
        ssize_t result = Pro_Wrapper::read_noeintr(filedes, buf, (nbyte - total));

        if (result < 0) {
            return result;
        }

        total += (size_t) result;
    }

    return (ssize_t) nbyte;
}
ssize_t BaseSocks::read_count(int filedes, char *buf, size_t nbyte)
{
    size_t total = 0;

    while (total < nbyte) {
        ssize_t result = Pro_Wrapper::read_noeintr(filedes, buf, (nbyte - total));

        if (result < 0) {
            return result;
        }

        total += (size_t) result;
    }

    return (ssize_t) nbyte;
}

ssize_t BaseSocks::socks_recv(void *buf, size_t bufsize)
{
	std::cout<<"sock recv :"<<std::endl;
    char header[2];
    uint16_t msgsize;
    ssize_t result;

    result = read_count(socket_fd, header, 2);

    if (result < 0) {
        return result;
    }

    msgsize = deserialize_uint16(header);

    if (msgsize > bufsize) {
        errno = EMSGSIZE;
        return -1;
    }

    return read_count(socket_fd, (char *) buf, msgsize);
}


ssize_t BaseSocks::write_count(int filedes, const char *buf, size_t nbyte)
{
    size_t remaining = nbyte;

    while (remaining != 0) {
        ssize_t result = Pro_Wrapper::write_noeintr(filedes, buf, remaining);

        if (result < 0) {
            return result;
        }

        remaining -= (size_t) result;
        buf += result;
    }

    return (ssize_t) nbyte;
}
int BaseSocks::fd_socket_setflag(int fd)
{
    return Pro_Wrapper::fcntl_setown_noeintr(fd,getpid());
}

int BaseSocks::fd_socket_setflag2(int fd)
{
    return Pro_Wrapper::fcntl_setown_noeintr(fd,getpid());
}

int BaseSocks::fd_socket_clearflag(int fd)
{
    return Pro_Wrapper::fcntl_setown_noeintr(fd, 0);
}
int BaseSocks::fd_socket_clearflag2(int fd)
{
    return Pro_Wrapper::fcntl_setown_noeintr(fd, 0);
}

int BaseSocks::fd_socket_checkflag(int fd)
{
    pid_t result = Pro_Wrapper::fcntl_getown_noeintr(fd);

    if (result != -1) {
        return (result != 0);
    }

    return -1;
}

int BaseSocks::fd_socket_checkflag2(int fd)
{
    pid_t result = Pro_Wrapper::fcntl_getown_noeintr(fd);

    if (result != -1) {
        return (result != 0);
    }

    return -1;
}
BaseSocks::BaseSocks() {
	// TODO Auto-generated constructor stub
	socket_fd = -1;
	channelname = "";
}
BaseSocks::~BaseSocks() {
	// TODO Auto-generated destructor stub
	socket_fd = -1;

}

int BaseSocks::socks_server_close()
{
    return Pro_Wrapper::close_noeintr(socket_fd);
}
int BaseSocks::socks_server_wait()
{
     int result = socks_server_select(NULL);

    if (result > 0) {
        return 0;
    }

    if (result == 0) {
        return -1;
    }

    return result;
}
int BaseSocks::socks_server_select(struct timeval *restrict timeout)
{
    fd_set read_fds;
    fd_set error_fds;

    FD_ZERO(&read_fds);
    FD_ZERO(&error_fds);
    FD_SET(socket_fd, &read_fds);
    FD_SET(socket_fd, &error_fds);

    int result = 0;

    result = Pro_Wrapper::select_noeintr(socket_fd + 1, &read_fds, NULL, &error_fds,timeout);

    /* We might normally use FD_ISSET here, but this isn't necessary
     * because we're only listening for one item (the socket). */

    if (result > 0) {
        return 1;
    }

    return result;
}
int BaseSocks::socks_server_poll()
{

	struct timeval nodelay;
	nodelay.tv_sec = 0;
	nodelay.tv_usec = 0;
	int result;
   result = socks_server_select(&nodelay);

    if (result > 0) {
        return 1;
    }

    return result;
}
int BaseSocks::socks_server_open(int socknum)
{


	if(socknum == 1)
	{
		 int sck_unix;
	    struct sockaddr_un adr_unix;
		 int len_unix;
   // const char pth_unix[] = "Z*MY-SOCKET-TQKJ*";
		 channelname = TYKJ_PROCESS_SOCKET1;
    const char pth_unix[] = TYKJ_PROCESS_SOCKET1;
    sck_unix = socket(AF_UNIX,SOCK_SEQPACKET,0);
    memset(&adr_unix,0,sizeof(adr_unix));
    adr_unix.sun_family = AF_UNIX;
    strncpy(adr_unix.sun_path,pth_unix,sizeof adr_unix.sun_path-1)[sizeof adr_unix.sun_path -1]=0;
  //   len_unix = SUN_LEN(&adr_unix);
    len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (adr_unix.sun_path);
     adr_unix.sun_path[0] = 0;
     bind(sck_unix,(struct sockaddr *)&adr_unix,len_unix);
    int result;
    result = listen(sck_unix, backlog_size);

    if (result != 0) {
        return -1;
    }
    socket_fd = sck_unix;
	}
   if(socknum == 2)
   {
		 int sck_unix2;
		    struct sockaddr_un adr_unix2;
			 int len_unix2;
	   // const char pth_unix[] = "Z*MY-SOCKET-TQKJ*";
			 channelname = TYKJ_PROCESS_SOCKET2;
	    const char pth_unix2[] = TYKJ_PROCESS_SOCKET2;
	    sck_unix2 = socket(AF_UNIX,SOCK_SEQPACKET,0);
	    memset(&adr_unix2,0,sizeof(adr_unix2));
	    adr_unix2.sun_family = AF_UNIX;
	    strncpy(adr_unix2.sun_path,pth_unix2,sizeof adr_unix2.sun_path-1)[sizeof adr_unix2.sun_path -1]=0;
	  //   len_unix = SUN_LEN(&adr_unix);
	    len_unix2 = offsetof(struct sockaddr_un, sun_path) + strlen (adr_unix2.sun_path);
	     adr_unix2.sun_path[0] = 0;
	     bind(sck_unix2,(struct sockaddr *)&adr_unix2,len_unix2);
 	    int result;
	    result = listen(sck_unix2, backlog_size);

	    if (result != 0) {
	        return -2;
	    }
	    socket_fd = sck_unix2;
   }

	if(socknum == 3)
	{
		 int sck_unix;
	    struct sockaddr_un adr_unix;
		 int len_unix;
  // const char pth_unix[] = "Z*MY-SOCKET-TQKJ*";
		 channelname = TYKJ_PROCESS_SOCKET3;
   const char pth_unix[] = TYKJ_PROCESS_SOCKET3;
   sck_unix = socket(AF_UNIX,SOCK_SEQPACKET,0);
   memset(&adr_unix,0,sizeof(adr_unix));
   adr_unix.sun_family = AF_UNIX;
   strncpy(adr_unix.sun_path,pth_unix,sizeof adr_unix.sun_path-1)[sizeof adr_unix.sun_path -1]=0;
 //   len_unix = SUN_LEN(&adr_unix);
   len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (adr_unix.sun_path);
    adr_unix.sun_path[0] = 0;
    bind(sck_unix,(struct sockaddr *)&adr_unix,len_unix);
   int result;
   result = listen(sck_unix, backlog_size);

   if (result != 0) {
       return -1;
   }
   socket_fd = sck_unix;
	}

	if(socknum == 4)
	{
		 int sck_unix;
	    struct sockaddr_un adr_unix;
		 int len_unix;
   // const char pth_unix[] = "Z*MY-SOCKET-TQKJ*";
		 channelname = TYKJ_PROCESS_SOCKET4;
    const char pth_unix[] = TYKJ_PROCESS_SOCKET4;
    sck_unix = socket(AF_UNIX,SOCK_SEQPACKET,0);
    memset(&adr_unix,0,sizeof(adr_unix));
    adr_unix.sun_family = AF_UNIX;
    strncpy(adr_unix.sun_path,pth_unix,sizeof adr_unix.sun_path-1)[sizeof adr_unix.sun_path -1]=0;
  //   len_unix = SUN_LEN(&adr_unix);
    len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (adr_unix.sun_path);
     adr_unix.sun_path[0] = 0;
     bind(sck_unix,(struct sockaddr *)&adr_unix,len_unix);
    int result;
    result = listen(sck_unix, backlog_size);

    if (result != 0) {
        return -1;
    }
    socket_fd = sck_unix;
	}
	if(socknum == 5)
	{
		 int sck_unix;
	    struct sockaddr_un adr_unix;
		 int len_unix;
   // const char pth_unix[] = "Z*MY-SOCKET-TQKJ*";
		 channelname = TYKJ_PROCESS_SOCKET5;
    const char pth_unix[] = TYKJ_PROCESS_SOCKET5;
    sck_unix = socket(AF_UNIX,SOCK_SEQPACKET,0);
    memset(&adr_unix,0,sizeof(adr_unix));
    adr_unix.sun_family = AF_UNIX;
    strncpy(adr_unix.sun_path,pth_unix,sizeof adr_unix.sun_path-1)[sizeof adr_unix.sun_path -1]=0;
  //   len_unix = SUN_LEN(&adr_unix);
    len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (adr_unix.sun_path);
     adr_unix.sun_path[0] = 0;
     bind(sck_unix,(struct sockaddr *)&adr_unix,len_unix);
    int result;
    result = listen(sck_unix, backlog_size);

    if (result != 0) {
        return -1;
    }
    socket_fd = sck_unix;
	}
	if(socknum == 6)
	{
		 int sck_unix;
	    struct sockaddr_un adr_unix;
		 int len_unix;
   // const char pth_unix[] = "Z*MY-SOCKET-TQKJ*";
		 channelname = TYKJ_PROCESS_SOCKET6;
    const char pth_unix[] = TYKJ_PROCESS_SOCKET6;
    sck_unix = socket(AF_UNIX,SOCK_SEQPACKET,0);
    memset(&adr_unix,0,sizeof(adr_unix));
    adr_unix.sun_family = AF_UNIX;
    strncpy(adr_unix.sun_path,pth_unix,sizeof adr_unix.sun_path-1)[sizeof adr_unix.sun_path -1]=0;
  //   len_unix = SUN_LEN(&adr_unix);
    len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (adr_unix.sun_path);
     adr_unix.sun_path[0] = 0;
     bind(sck_unix,(struct sockaddr *)&adr_unix,len_unix);
    int result;
    result = listen(sck_unix, backlog_size);

    if (result != 0) {
        return -1;
    }
    socket_fd = sck_unix;
	}
	if(socknum == 7)
	{
		 int sck_unix;
	    struct sockaddr_un adr_unix;
		 int len_unix;
   // const char pth_unix[] = "Z*MY-SOCKET-TQKJ*";
		 channelname = TYKJ_PROCESS_SOCKET7;
    const char pth_unix[] = TYKJ_PROCESS_SOCKET7;
    sck_unix = socket(AF_UNIX,SOCK_SEQPACKET,0);
    memset(&adr_unix,0,sizeof(adr_unix));
    adr_unix.sun_family = AF_UNIX;
    strncpy(adr_unix.sun_path,pth_unix,sizeof adr_unix.sun_path-1)[sizeof adr_unix.sun_path -1]=0;
  //   len_unix = SUN_LEN(&adr_unix);
    len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (adr_unix.sun_path);
     adr_unix.sun_path[0] = 0;
     bind(sck_unix,(struct sockaddr *)&adr_unix,len_unix);
    int result;
    result = listen(sck_unix, backlog_size);

    if (result != 0) {
        return -1;
    }
    socket_fd = sck_unix;
	}
   std::cout<<"soket_fd is "<<socket_fd<<std::endl;
   return socket_fd;
}
ssize_t BaseSocks::socks_server_respond(int response_fd,const void *buf, uint16_t nbyte)
{
    if (fd_socket_setflag(response_fd) != 0) {
        fprintf(stderr, "setflag failed!\n");
    }

    return socks_send(response_fd,buf, nbyte);
}

ssize_t BaseSocks::socks_server_respond2(int response_fd, const void *buf, uint16_t nbyte)
{

    if (fd_socket_setflag2(response_fd) != 0) {
        fprintf(stderr, "setflag failed!\n");
    }
     std::cout<<"socks_server_respond2 is "<<response_fd<<std::endl;
    return socks_send2(response_fd, buf, nbyte);
}
ssize_t BaseSocks::socks_send(int fd,const void *buf, uint16_t nbyte)
{
    char header[2];
    ssize_t result;

    serialize_uint16(nbyte, header);
    result = write_count(fd, header, 2);

    if (result < 0) {
        return result;
    }

    return write_count(fd, (const char *) buf, nbyte);
}
ssize_t BaseSocks::socks_send2(int fd, const void *buf, uint16_t nbyte)
{
    char header[2];
    ssize_t result;

    serialize_uint16_2(nbyte, header);
    result = write_count2(fd, header, 2);

    if (result < 0) {
        return result;
    }

    return write_count2(fd, (const char *) buf, nbyte);
}
int BaseSocks::socks_process_request(int socket_fd1,socks_callback_t callback,
                                 uint16_t input_size)
{
    int result;

    char buffer[1024];
    int callback_result;

    buffer[input_size] = '\x00';

    result = (int) read_count(socket_fd1, buffer, input_size);

    if (result < 0) {
        return result;
    }

    result = fd_socket_clearflag(socket_fd1);

    if (result < 0) {
        fprintf(stderr, "couldn't clear flag\n");
        return result;
    }

    callback_result = callback(*this,socket_fd1, buffer, input_size);

    switch (fd_socket_checkflag(socket_fd1)) {
        case 0:
            result = (int) socks_server_respond(socket_fd1,"", 0);
            break;

        case 1:
            break;

        default:
            fprintf(stderr, "couldn't check flag\n");
            break;
    }

    if (result == 0) {
        return callback_result;
    }

    return result;
}
int BaseSocks::socks_process_request2(BaseSocks basesock,int socket_fd,socks_callback_t callback,
                                 uint16_t input_size)
{

    int result;

    char buffer[1024];
    int callback_result;

    buffer[input_size] = '\x00';

    result = (int) read_count2(socket_fd, buffer, input_size);

    if (result < 0) {
        return result;
    }

    result = fd_socket_clearflag2(socket_fd);

    if (result < 0) {
        fprintf(stderr, "couldn't clear flag\n");
        return result;
    }


    callback_result = callback(basesock,socket_fd, buffer, input_size);

    switch (fd_socket_checkflag2(socket_fd)) {
        case 0:
            result = (int) socks_server_respond2(socket_fd,"", strlen(""));
            break;

        case 1:
            break;

        default:
            fprintf(stderr, "couldn't check flag\n");
            break;
    }

    if (result == 0) {
        return callback_result;
    }

    return result;
}
int BaseSocks::socks_server_process2(BaseSocks basesock,int socket_fd, socks_callback_t callback)
{
    int connection_fd;
    ssize_t result;
    char header[2];
    uint16_t msgsize;

    connection_fd = Pro_Wrapper::accept_noeintr(socket_fd, NULL, NULL);

    if (connection_fd < 0) {
        return connection_fd;
    }

    result = read_count2(connection_fd, header, 2);

    if (result < 0) {
        return (int) result;
    }

    msgsize = deserialize_uint16_2(header);
    result = socks_process_request2(basesock,connection_fd, callback, msgsize);
    Pro_Wrapper::close_noeintr(connection_fd);

    return (int) result;
}
int BaseSocks::socks_server_process()
{
    int connection_fd;
    ssize_t result;
    char header[2];
    uint16_t msgsize;

    connection_fd = Pro_Wrapper::accept_noeintr(socket_fd, NULL, NULL);

    if (connection_fd < 0) {
        return connection_fd;
    }

    result = read_count(connection_fd, header, 2);

    if (result < 0) {
        return (int) result;
    }

    msgsize = deserialize_uint16(header);
    result = socks_process_request(connection_fd, m_fun, msgsize);
    Pro_Wrapper::close_noeintr(connection_fd);

    return (int) result;
}


ssize_t BaseSocks::socks_client_process(const char *filename, const char *input,
                             uint16_t nbyte, char *output, uint16_t maxlen)
{
    ssize_t result;
    int socket_fd;
    struct sockaddr_un address;
       size_t len_unix = 0;
      const char pth_unix[] = "Z*MY-SOCKET-TQKJ_001*";
      socket_fd = socket(AF_UNIX,SOCK_SEQPACKET,0);
      memset(&address,0,sizeof(address));
      address.sun_family = AF_UNIX;
      strncpy(address.sun_path,pth_unix,sizeof address.sun_path-1)[sizeof address.sun_path -1]=0;
    //   len_unix = SUN_LEN(&adr_unix);
      len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (address.sun_path);
      address.sun_path[0] = 0;


    if (socket_fd < 0) {
        fprintf(stderr, "Couldn't open socket [%s]\n", filename);
        return socket_fd;
    }

    result = Pro_Wrapper::connect_noeintr(socket_fd, (struct sockaddr *) &address,len_unix);

    if (result != 0) {
        fprintf(stderr, "Couldn't connect to socket [%s]\n", filename);
        Pro_Wrapper::close_noeintr(socket_fd);
        return result;
    }

    result = socks_send(socket_fd,input, nbyte);

    if (result < 0) {
    	Pro_Wrapper::close_noeintr(socket_fd);
        return result;
    }

    result = socks_recv(output, maxlen);
    Pro_Wrapper::close_noeintr(socket_fd);
    return result;
}
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
void * BaseSocks::handler()
{
	return this;
}
void BaseSocks::handler_static(void*p_A,void* pC)
{
	BaseSocks* _p =static_cast<BaseSocks*>(p_A);

	pC = _p->handler();
}
int BaseSocks::Get_SocketFd()
{
	return socket_fd;
}
string BaseSocks::Get_ChannelName()
{
	return channelname;
}
int BaseSocks::run(int socknum,socks_callback_t callback)
{
	int result;
	int socks_fd;
	socks_fd = socks_server_open(socknum);
	if (socks_fd < 0) {
	        perror(NULL);
	        exit(socks_fd);
	    }
	while (1) {
	        if (shutdowning) {
	            break;
	        }

	        if (blocking) {
	                  result = socks_server_wait();

	                  if (result != 0) {
	                      fprintf(stderr, "socks_server_wait 1: failed (%s)\n",
	                              strerror(errno));
	                      return result;
	                  }
	              }
	        else {
	                   while (1) {
	                       result = socks_server_poll();

	                       if (result < 0) {
	                           fprintf(stderr, "socks_server_wait 2: failed (%s)\n",
	                                   strerror(errno));
	                           return result;
	                       }

	                       if (result) {
	                           break;
	                       }
	                       sleep_ms(2);
	                   }
	        }

	        result =socks_server_process2(*this,socks_fd,callback);
	      //   result2 = BaseSocks::socks_server_process2(socks_fd2,callback);
	        if (result != 0) {
	            if (errno != 0) {
	                fprintf(stderr, "socks_server_process 1: failed (%s)\n",
	                        strerror(errno));
	                return result;
	            }
	            fprintf(stderr, "warn: command failed with code %d\n", result);
	        }
	    }


	    if (result != 0) {
	        perror(NULL);
	    }

	    result = socks_server_close();

	    return result;

}

int BaseSocks::run2(int socknum)
{
	int result;
	int socks_fd;
	socks_fd = socks_server_open(socknum);
	std::cout<<"dfsdf222"<<std::endl;
	if (socks_fd < 0) {
		std::cout<<"error"<<std::endl;
	        perror(NULL);
	        exit(socks_fd);

	    }
	while (1) {
	        if (shutdowning) {
	            break;
	        }

	        if (blocking) {
	                  result = socks_server_wait();

	                  if (result != 0) {
	                      fprintf(stderr, "socks_server_wait 1: failed (%s)\n",
	                              strerror(errno));
	                      return result;
	                  }
	              }
	        else {
	                   while (1) {
	                       result = socks_server_poll();

	                       if (result < 0) {
	                           fprintf(stderr, "socks_server_wait 2: failed (%s)\n",
	                                   strerror(errno));
	                           return result;
	                       }

	                       if (result) {
	                           break;
	                       }
	                       sleep_ms(2);
	                   }
	        }
              std::cout<<"1234234"<<std::endl;
	        result =socks_server_process();
	      //   result2 = BaseSocks::socks_server_process2(socks_fd2,callback);
	        if (result != 0) {
	            if (errno != 0) {
	                fprintf(stderr, "socks_server_process 1: failed (%s)\n",
	                        strerror(errno));
	                return result;
	            }
	            fprintf(stderr, "warn: command failed with code %d\n", result);
	        }
	    }


	    if (result != 0) {
	        perror(NULL);
	    }

	    result = socks_server_close();

	    return result;

}






