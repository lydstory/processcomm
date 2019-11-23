/*
 * SocksClient.cpp
 *
 *  Created on: 2019年8月17日
 *      Author: root
 */
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
#include "SocksClient.h"
#include "ProWrapper.h"
#include "Tqtype.h"
SocksClient::SocksClient() {
	// TODO Auto-generated constructor stub

}

SocksClient::~SocksClient() {
	// TODO Auto-generated destructor stub
}
void SocksClient::serialize_uint16(uint16_t input, char output[2])
{
    output[0] = (char)((input >> 0) & 0xFF);
    output[1] = (char)((input >> 8) & 0xFF);
}

/** @brief Deserializes a little-endian 2-char array and returns the result.
 * Used to ensure predictable deserialization across platforms.
 * @param[in] input Serialized data
 * @return Deserialized uint16 */
uint16_t SocksClient::deserialize_uint16(const char input[2])
{
    int result = 0;

    result += (input[0]) << 0;
    result += (input[1]) << 8;

    return (uint16_t) (result & 0xFFFF);
}
int SocksClient::socks_client_open(int socknum)
{
	 int sck_unix;
	  struct sockaddr_un address;

	   size_t len_unix = 0;
	if(socknum == 1)
	{
		  ssize_t result;
	     int sc_soclinux;

		  const char pth_unix[] = TYKJ_PROCESS_SOCKET1;
		  sc_soclinux = socket(AF_UNIX,SOCK_SEQPACKET,0);
		  memset(&address,0,sizeof(address));
		  address.sun_family = AF_UNIX;
		  strncpy(address.sun_path,pth_unix,sizeof address.sun_path-1)[sizeof address.sun_path -1]=0;
		//   len_unix = SUN_LEN(&adr_unix);
		  len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (address.sun_path);
		  address.sun_path[0] = 0;


		if (sc_soclinux < 0) {

		    return sc_soclinux;
		}

		result = Pro_Wrapper::connect_noeintr(sc_soclinux, (struct sockaddr *) &address,len_unix);
		  if (result != 0) {

		        Pro_Wrapper::close_noeintr(sc_soclinux);
		        return result;
		    }
		  socket_fd = sc_soclinux;
	}
   if(socknum == 2)
   {
		  ssize_t result;
		      int sc_soclinux;

			  const char pth_unix[] = TYKJ_PROCESS_SOCKET2;
			  sc_soclinux = socket(AF_UNIX,SOCK_SEQPACKET,0);
			  memset(&address,0,sizeof(address));
			  address.sun_family = AF_UNIX;
			  strncpy(address.sun_path,pth_unix,sizeof address.sun_path-1)[sizeof address.sun_path -1]=0;
			//   len_unix = SUN_LEN(&adr_unix);
			  len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (address.sun_path);
			  address.sun_path[0] = 0;


			if (sc_soclinux < 0) {

			    return sc_soclinux;
			}

			result = Pro_Wrapper::connect_noeintr(sc_soclinux, (struct sockaddr *) &address,len_unix);
			  if (result != 0) {

			        Pro_Wrapper::close_noeintr(sc_soclinux);
			        return result;
			    }
			  socket_fd = sc_soclinux;
   }
   if(socknum == 3)
   {
		  ssize_t result;
		      int sc_soclinux;

			  const char pth_unix[] = TYKJ_PROCESS_SOCKET3;
			  sc_soclinux = socket(AF_UNIX,SOCK_SEQPACKET,0);
			  memset(&address,0,sizeof(address));
			  address.sun_family = AF_UNIX;
			  strncpy(address.sun_path,pth_unix,sizeof address.sun_path-1)[sizeof address.sun_path -1]=0;
			//   len_unix = SUN_LEN(&adr_unix);
			  len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (address.sun_path);
			  address.sun_path[0] = 0;


			if (sc_soclinux < 0) {

			    return sc_soclinux;
			}

			result = Pro_Wrapper::connect_noeintr(sc_soclinux, (struct sockaddr *) &address,len_unix);
			  if (result != 0) {

			        Pro_Wrapper::close_noeintr(sc_soclinux);
			        return result;
			    }
			  socket_fd = sc_soclinux;
   }
   if(socknum == 4)
   {
		  ssize_t result;
		      int sc_soclinux;

			  const char pth_unix[] = TYKJ_PROCESS_SOCKET4;
			  sc_soclinux = socket(AF_UNIX,SOCK_SEQPACKET,0);
			  memset(&address,0,sizeof(address));
			  address.sun_family = AF_UNIX;
			  strncpy(address.sun_path,pth_unix,sizeof address.sun_path-1)[sizeof address.sun_path -1]=0;
			//   len_unix = SUN_LEN(&adr_unix);
			  len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (address.sun_path);
			  address.sun_path[0] = 0;


			if (sc_soclinux < 0) {

			    return sc_soclinux;
			}

			result = Pro_Wrapper::connect_noeintr(sc_soclinux, (struct sockaddr *) &address,len_unix);
			  if (result != 0) {

			        Pro_Wrapper::close_noeintr(sc_soclinux);
			        return result;
			    }
			  socket_fd = sc_soclinux;
   }
   if(socknum == 5)
   {
		  ssize_t result;
		      int sc_soclinux;

			  const char pth_unix[] = TYKJ_PROCESS_SOCKET5;
			  sc_soclinux = socket(AF_UNIX,SOCK_SEQPACKET,0);
			  memset(&address,0,sizeof(address));
			  address.sun_family = AF_UNIX;
			  strncpy(address.sun_path,pth_unix,sizeof address.sun_path-1)[sizeof address.sun_path -1]=0;
			//   len_unix = SUN_LEN(&adr_unix);
			  len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (address.sun_path);
			  address.sun_path[0] = 0;


			if (sc_soclinux < 0) {

			    return sc_soclinux;
			}

			result = Pro_Wrapper::connect_noeintr(sc_soclinux, (struct sockaddr *) &address,len_unix);
			  if (result != 0) {

			        Pro_Wrapper::close_noeintr(sc_soclinux);
			        return result;
			    }
			  socket_fd = sc_soclinux;
   }
   if(socknum == 6)
   {
		  ssize_t result;
		      int sc_soclinux;

			  const char pth_unix[] = TYKJ_PROCESS_SOCKET6;
			  sc_soclinux = socket(AF_UNIX,SOCK_SEQPACKET,0);
			  memset(&address,0,sizeof(address));
			  address.sun_family = AF_UNIX;
			  strncpy(address.sun_path,pth_unix,sizeof address.sun_path-1)[sizeof address.sun_path -1]=0;
			//   len_unix = SUN_LEN(&adr_unix);
			  len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (address.sun_path);
			  address.sun_path[0] = 0;


			if (sc_soclinux < 0) {

			    return sc_soclinux;
			}

			result = Pro_Wrapper::connect_noeintr(sc_soclinux, (struct sockaddr *) &address,len_unix);
			  if (result != 0) {

			        Pro_Wrapper::close_noeintr(sc_soclinux);
			        return result;
			    }
			  socket_fd = sc_soclinux;
   }
   if(socknum == 7)
     {
  		  ssize_t result;
  		      int sc_soclinux;

  			  const char pth_unix[] = TYKJ_PROCESS_SOCKET7;
  			  sc_soclinux = socket(AF_UNIX,SOCK_SEQPACKET,0);
  			  memset(&address,0,sizeof(address));
  			  address.sun_family = AF_UNIX;
  			  strncpy(address.sun_path,pth_unix,sizeof address.sun_path-1)[sizeof address.sun_path -1]=0;
  			//   len_unix = SUN_LEN(&adr_unix);
  			  len_unix = offsetof(struct sockaddr_un, sun_path) + strlen (address.sun_path);
  			  address.sun_path[0] = 0;


  			if (sc_soclinux < 0) {

  			    return sc_soclinux;
  			}

  			result = Pro_Wrapper::connect_noeintr(sc_soclinux, (struct sockaddr *) &address,len_unix);
  			  if (result != 0) {

  			        Pro_Wrapper::close_noeintr(sc_soclinux);
  			        return result;
  			    }
  			  socket_fd = sc_soclinux;
     }
	   return socket_fd;
}
ssize_t SocksClient::read_count(int filedes, char *buf, size_t nbyte)
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

ssize_t SocksClient::socks_recv(void *buf, size_t bufsize)
{

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
ssize_t SocksClient::write_count(int filedes, const char *buf, size_t nbyte)
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

ssize_t SocksClient::socks_send(const void *buf, uint16_t nbyte)
{
    char header[2];
    ssize_t result;

    serialize_uint16(nbyte, header);
    result = write_count(socket_fd, header, 2);

    if (result < 0) {
        return result;
    }

    return write_count(socket_fd, (const char *) buf, nbyte);
}

ssize_t SocksClient::socks_client_process(const char *filename, const char *input,
                             uint16_t nbyte, char *output, uint16_t maxlen)
{
    ssize_t result;
    int socket_fd;
    struct sockaddr_un address;
       size_t len_unix = 0;
      const char pth_unix[] = TYKJ_PROCESS_SOCKET2;
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

    result = socks_send(input, nbyte);

    if (result < 0) {
    	Pro_Wrapper::close_noeintr(socket_fd);
        return result;
    }

    result = socks_recv(output, maxlen);
    Pro_Wrapper::close_noeintr(socket_fd);
    return result;
}

