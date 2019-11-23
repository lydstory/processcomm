/*
 * SocksClient.h
 *
 *  Created on: 2019年8月17日
 *      Author: root
 */

#ifndef SOCKSCLIENT_H_
#define SOCKSCLIENT_H_
#include <stdint.h>
class SocksClient {
public:
	SocksClient();
	virtual ~SocksClient();
	int socks_client_open(int socknum);
	ssize_t socks_recv(void *buf, size_t bufsize);
	ssize_t read_count(int filedes, char *buf, size_t nbyte);
	ssize_t socks_send(const void *buf, uint16_t nbyte);
	ssize_t write_count(int filedes, const char *buf, size_t nbyte);
	void serialize_uint16(uint16_t input, char output[2]);
	uint16_t deserialize_uint16(const char input[2]);
	ssize_t socks_client_process(const char *filename, const char *input,
	                                uint16_t nbyte, char *output, uint16_t maxlen);
private:
	int socket_fd;
};

#endif /* SOCKSCLIENT_H_ */
