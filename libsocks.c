#define _POSIX_C_SOURCE 200809L

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

#include "eintr_wrappers.h"
#include "libsocks.h"
#define restrict __restrict

/*----------------------------------------------------------------------------*/

/** @brief Serializes a uint16_t into a little-endian 2-char array. Used to
 * ensure predictable serialization across platforms.
 * @param[in] input Value to serialize
 * @param[out] output Destination for serialized data */
static void serialize_uint16(uint16_t input, char output[2])
{
    output[0] = (char)((input >> 0) & 0xFF);
    output[1] = (char)((input >> 8) & 0xFF);
}

/** @brief Deserializes a little-endian 2-char array and returns the result.
 * Used to ensure predictable deserialization across platforms.
 * @param[in] input Serialized data
 * @return Deserialized uint16 */
static uint16_t deserialize_uint16(const char input[2])
{
    int result = 0;

    result += (input[0]) << 0;
    result += (input[1]) << 8;

    return (uint16_t) (result & 0xFFFF);
}

/** @brief Reads a specified number of bytes from a file-descriptor. Retries
 * until enough bytes are received (or until the read command fails).
 * @param[in] filedes File descriptor used to read data
 * @param[out] buf Pointer to target data buffer
 * @param[in] nbyte Number of bytes to read
 * @return Number of bytes retrieved, or -1 in the event of an error.
 * @retval <0 A read error occured, and errno was set accordingly.
 * @retval >=0 Number of bytes retrieved. */
static ssize_t read_count(int filedes, char *buf, size_t nbyte)
{
    size_t total = 0;

    while (total < nbyte) {
        ssize_t result = read_noeintr(filedes, buf, (nbyte - total));

        if (result < 0) {
            return result;
        }

        total += (size_t) result;
    }

    return (ssize_t) nbyte;
}

/** @brief Writes a specified number of bytes to a file-descriptor. Retries
 * until enough bytes are written (or until the write command fails).
 * @param[in] filedes File descriptor used to write data
 * @param[in] buf Pointer to input data buffer
 * @param[in] nbyte Number of bytes to write
 * @return Number of bytes written, or -1 in the event of an error.
 * @retval <0 A write error occured, and errno was set accordingly.
 * @retval >=0 Number of bytes written. */
static ssize_t write_count(int filedes, const char *buf, size_t nbyte)
{
    size_t remaining = nbyte;

    while (remaining != 0) {
        ssize_t result = write_noeintr(filedes, buf, remaining);

        if (result < 0) {
            return result;
        }

        remaining -= (size_t) result;
        buf += result;
    }

    return (ssize_t) nbyte;
}

static int fd_socket_setflag(int fd)
{
    return fcntl_setown_noeintr(fd, getpid());
}

static int fd_socket_clearflag(int fd)
{
    return fcntl_setown_noeintr(fd, 0);
}

static int fd_socket_checkflag(int fd)
{
    pid_t result = fcntl_getown_noeintr(fd);

    if (result != -1) {
        return (result != 0);
    }

    return -1;
}

/*----------------------------------------------------------------------------*/

#define get_size(type, field) sizeof(((type *)0)->field)

enum {
    sun_path_size = get_size(struct sockaddr_un, sun_path) - 1,
    address_maxlen = (sun_path_size > PATH_MAX) ? sun_path_size : PATH_MAX,
    backlog_target = 16,
    backlog_size = (backlog_target < SOMAXCONN) ? backlog_target : SOMAXCONN
};


static ssize_t socks_recv(int fd, void *buf, size_t bufsize)
{
    char header[2];
    uint16_t msgsize;
    ssize_t result;

    result = read_count(fd, header, 2);

    if (result < 0) {
        return result;
    }

    msgsize = deserialize_uint16(header);

    if (msgsize > bufsize) {
        errno = EMSGSIZE;
        return -1;
    }

    return read_count(fd, (char *) buf, msgsize);
}

static ssize_t socks_send(int fd, const void *buf, uint16_t nbyte)
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

static int socks_process_request(int connection_fd, socks_callback_t callback,
                                 uint16_t input_size)
{
    int result;

    char buffer[1024];
    int callback_result;

    buffer[input_size] = '\x00';

    result = (int) read_count(connection_fd, buffer, input_size);

    if (result < 0) {
        return result;
    }

    result = fd_socket_clearflag(connection_fd);

    if (result < 0) {
        fprintf(stderr, "couldn't clear flag\n");
        return result;
    }

    callback_result = callback(connection_fd, buffer, input_size);

    switch (fd_socket_checkflag(connection_fd)) {
        case 0:
            result = (int) socks_server_respond(connection_fd, "", 0);
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

static int socks_server_select(int socket_fd, struct timeval *restrict timeout)
{
    fd_set read_fds;
    fd_set error_fds;

    FD_ZERO(&read_fds);
    FD_ZERO(&error_fds);
    FD_SET(socket_fd, &read_fds);
    FD_SET(socket_fd, &error_fds);

    int result = 0;

    result = select_noeintr(socket_fd + 1, &read_fds, NULL, &error_fds,timeout);

    /* We might normally use FD_ISSET here, but this isn't necessary
     * because we're only listening for one item (the socket). */

    if (result > 0) {
        return 1;
    }

    return result;
}

/*----------------------------------------------------------------------------*/

ssize_t socks_server_respond(int response_fd, const void *buf, uint16_t nbyte)
{
    if (fd_socket_setflag(response_fd) != 0) {
        fprintf(stderr, "setflag failed!\n");
    }

    return socks_send(response_fd, buf, nbyte);
}

//int socks_server_open(const char *filename, mode_t mode)
int socks_server_open()
{
    int sck_unix;
    struct sockaddr_un adr_unix;
    int len_unix;
    const char pth_unix[] = "Z*MY-SOCKET-TQKJ*";
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

    return sck_unix;
}

int socks_server_process(int socket_fd, socks_callback_t callback)
{
    int connection_fd;
    ssize_t result;
    char header[2];
    uint16_t msgsize;

    connection_fd = accept_noeintr(socket_fd, NULL, NULL);

    if (connection_fd < 0) {
        return connection_fd;
    }

    result = read_count(connection_fd, header, 2);

    if (result < 0) {
        return (int) result;
    }

    msgsize = deserialize_uint16(header);
    result = socks_process_request(connection_fd, callback, msgsize);
    close_noeintr(connection_fd);

    return (int) result;
}

int socks_server_poll(int socket_fd)
{

	struct timeval nodelay;
	nodelay.tv_sec = 0;
	nodelay.tv_usec = 0;
	int result;
	result = socks_server_select(socket_fd, &nodelay);

    if (result > 0) {
        return 1;
    }

    return result;
}

int socks_server_wait(int socket_fd)
{
    int result = socks_server_select(socket_fd, NULL);

    if (result > 0) {
        return 0;
    }

    if (result == 0) {
        return -1;
    }

    return result;
}

int socks_server_close(int socket_fd)
{
    return close_noeintr(socket_fd);
}

/*----------------------------------------------------------------------------*/

ssize_t socks_client_process(const char *filename, const char *input,
                             uint16_t nbyte, char *output, uint16_t maxlen)
{
    ssize_t result;
    int socket_fd;
    struct sockaddr_un address;
       size_t len_unix = 0;
      const char pth_unix[] = "Z*MY-SOCKET-TYKJ*";
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

    result = connect_noeintr(socket_fd, (struct sockaddr *) &address,len_unix);

    if (result != 0) {
        fprintf(stderr, "Couldn't connect to socket [%s]\n", filename);
        close_noeintr(socket_fd);
        return result;
    }

    result = socks_send(socket_fd, input, nbyte);

    if (result < 0) {
        close_noeintr(socket_fd);
        return result;
    }

    result = socks_recv(socket_fd, output, maxlen);
    close_noeintr(socket_fd);
    return result;
}
