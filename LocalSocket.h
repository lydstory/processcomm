/*
 * LocalSocket.h
 *
 *  Created on: 2019年8月20日
 *      Author: root
 */

#ifndef LOCALSOCKET_H_
#define LOCALSOCKET_H_
#include "BaseSocks.h"
#include "SocksClient.h"
class LocalSocket {
public:
	LocalSocket();
	virtual ~LocalSocket();
	int init(int socknum);
private:
	BaseSocks servsock;
	SocksClient clientsock;

};

#endif /* LOCALSOCKET_H_ */
