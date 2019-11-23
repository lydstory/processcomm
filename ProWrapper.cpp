/*
 * ProWrapper.cpp
 *
 *  Created on: 2019年8月17日
 *      Author: lyd
 */

#include "ProWrapper.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#define wrap_call(dtype, x) \
    do { \
        dtype result; \
        do { \
            result = x; \
        } while ((result == -1) && (errno == EINTR)); \
        return result; \
    } while (0)

Pro_Wrapper::Pro_Wrapper() {
	// TODO Auto-generated constructor stub

}

Pro_Wrapper::~Pro_Wrapper() {
	// TODO Auto-generated destructor stub
}

int Pro_Wrapper::accept_noeintr(int socket, struct sockaddr *restrict address,socklen_t *restrict address_len)
{
	wrap_call(int, accept(socket, address, address_len));
}
int Pro_Wrapper::close_noeintr(int fildes)
{
	 wrap_call(int, close(fildes));
}
int Pro_Wrapper::connect_noeintr(int socket, const struct sockaddr *address,
            socklen_t address_len)
{
    wrap_call(int, connect(socket, address, address_len));
}
int Pro_Wrapper::fcntl_setown_noeintr(int fildes, pid_t owner)
{
	  wrap_call(int, fcntl(fildes, F_SETOWN, owner));
}
pid_t Pro_Wrapper::fcntl_getown_noeintr(int fildes)
{
	   wrap_call(pid_t, fcntl(fildes, F_GETOWN, 0));
}
ssize_t Pro_Wrapper::read_noeintr(int fildes, void *buf, size_t nbyte)
{
    wrap_call(ssize_t, read(fildes, buf, nbyte));
}
int Pro_Wrapper::select_noeintr(int nfds, fd_set *restrict readfds,
                   fd_set *restrict writefds, fd_set *restrict errorfds,
                   struct timeval *restrict timeout)
{
	wrap_call(int, select(nfds, readfds, writefds, errorfds, timeout));
}
int Pro_Wrapper::chmod_noeintr(const char *path, mode_t mode)
{
    wrap_call(int, chmod(path, mode));
}
int Pro_Wrapper::chown_noeintr(const char *path, uid_t owner, gid_t group)
{
    wrap_call(int, chown(path, owner, group));
}
int Pro_Wrapper::closedir_noeintr(DIR *dirp)
{
    wrap_call(int, closedir(dirp));
}

int Pro_Wrapper::fchdir_noeintr(int fildes)
{
    wrap_call(int, fchdir(fildes));
}
ssize_t Pro_Wrapper::write_noeintr(int fildes, const void *buf, size_t nbyte)
{
	  wrap_call(ssize_t, write(fildes, buf, nbyte));
}
