/*
 * ProWrapper.h
 *
 *  Created on: 2019年8月17日
 *      Author: lyd
 */

#ifndef PROWRAPPER_H_
#define PROWRAPPER_H_

#include <dirent.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#define restrict __restrict

class Pro_Wrapper {
public:
	Pro_Wrapper();
	virtual ~Pro_Wrapper();
//static method
	static int accept_noeintr(int socket, struct sockaddr *restrict address,
	                   socklen_t *restrict address_len);

	static int close_noeintr(int fildes);

	static int connect_noeintr(int socket, const struct sockaddr *address,
            socklen_t address_len);
	static int fcntl_setown_noeintr(int fildes, pid_t owner);
	static pid_t fcntl_getown_noeintr(int fildes);
	static ssize_t read_noeintr(int fildes, void *buf, size_t nbyte);
	static int select_noeintr(int nfds, fd_set *restrict readfds,
            fd_set *restrict writefds, fd_set *restrict errorfds,
            struct timeval *restrict timeout);
	static int chmod_noeintr(const char *path, mode_t mode);
	static int chown_noeintr(const char *path, uid_t owner, gid_t group);
	static int closedir_noeintr(DIR *dirp);
	static int fchdir_noeintr(int fildes);
	static ssize_t write_noeintr(int fildes, const void *buf, size_t nbyte);
};

#endif /* PROWRAPPER_H_ */
