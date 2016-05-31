/*
 *   Copyright (c) 2016 Kulykov Oleh <info@resident.name>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */


#ifndef __NHR_REQUEST_H__
#define __NHR_REQUEST_H__ 1

#include "../libnhr.h"
#include "nhr_common.h"
#include "nhr_thread.h"
#include "nhr_memory.h"
#include "nhr_string.h"
#include "nhr_response.h"

#if defined(NHR_OS_WINDOWS)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <assert.h>
#include <errno.h>

#if defined(NHR_OS_WINDOWS)
typedef SOCKET nhr_socket_t;
#define NHR_INVALID_SOCKET INVALID_SOCKET
#define NHR_SOCK_CLOSE(sock) closesocket(sock)
#else
typedef int nhr_socket_t;
#define NHR_INVALID_SOCKET -1
#define NHR_SOCK_CLOSE(sock) close(sock)
#endif

//static const char * k_rws_socket_min_http_ver = "1.1";
//static const char * k_rws_socket_sec_websocket_accept = "Sec-WebSocket-Accept";

typedef struct _nhr_request_struct
{
	unsigned short port;

	nhr_socket_t socket;
	char * scheme;
	char * host;
	char * path;
	nhr_method method;
	
	nhr_thread work_thread;

	int command;

	nhr_error_code error_code;

	void * user_object;

	char * http_headers;
	char * parameters;

	nhr_on_request_recvd_responce on_recvd_responce;
	nhr_on_request_error on_error;

	nhr_mutex work_mutex;
	nhr_mutex send_mutex;

	_nhr_response * responce;
} _nhr_request;

nhr_bool nhr_request_send_buffer(_nhr_request * r, const void * data, const size_t data_size);

void nhr_request_wait_raw_responce(_nhr_request * r);

void nhr_request_send_raw_request(_nhr_request * r);

struct addrinfo * nhr_request_connect_getaddr_info(_nhr_request * r);

void nhr_request_connect_to_host(_nhr_request * r);

nhr_bool nhr_request_create_start_work_thread(_nhr_request * r);

void nhr_request_close(_nhr_request * r);

void nhr_request_delete(_nhr_request * r);

void nhr_request_set_option(nhr_socket_t s, int option, int value);

#define COMMAND_IDLE -1
#define COMMAND_NONE 0
#define COMMAND_CONNECT_TO_HOST 1
#define COMMAND_SEND_RAW_REQUEST 2
#define COMMAND_WAIT_RAW_RESPONCE 3

#define COMMAND_INFORM_RESPONCE 4
#define COMMAND_INFORM_ERROR 5

#define COMMAND_END 9999

#endif


// request/responce body
// https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol