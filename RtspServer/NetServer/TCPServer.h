/*
 * TCPServer.h
 *
 *  Created on: 2014年10月26日
 *      Author: liuxiaopei
 */
#ifndef NETSERVER_TCPSERVER_H_
#define NETSERVER_TCPSERVER_H_
#include "../libuv/include/uv.h"
#include <string>


typedef void (*CONNECTIION_CALLBACK)(uv_stream_t* stream, void* userData);

typedef void (*READ_CALLBACK)(uv_stream_t* stream,
                              ssize_t nread,
                              const uv_buf_t* buf);

typedef struct STREAM_PARAM{
    void* userData;
    char* recvBuffer;
    int32_t bufferLen;
    READ_CALLBACK pReadCallBack;
}STREAM_PARAM;


class TCPServer {
public:
	TCPServer();
	virtual ~TCPServer();

public:
    static void connection_cb(uv_stream_t* stream, int status);

    static void after_alloc(uv_handle_t* handle,
                            size_t suggested_size,
                            uv_buf_t* buf);

    static void after_read(uv_stream_t* stream,
                           ssize_t nread,
                           const uv_buf_t* buf);
public:
    void SetNetServerInfo(int32_t nPort, const char* szIpAddress);

    int InitNetServer();

    void SetConnectCallback(CONNECTIION_CALLBACK connectionCb, void* userData);

protected:
    void processConnection(uv_stream_t* stream, int status);
private:
    std::string m_strLocalIp;
    int32_t m_nPort;

    uv_loop_t*  m_uvLoop;
    uv_tcp_t    m_uvTcp;

    CONNECTIION_CALLBACK pConnectionCb;
    void*   pUserData;
};

#endif /* NETSERVER_TCPSERVER_H_ */
