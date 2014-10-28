/*
 * TCPServer.cpp
 *
 *  Created on: 2014年10月26日
 *      Author: liuxiaopei
 */

#include "TCPServer.h"
#include <string.h>

TCPServer::TCPServer():pConnectionCb(NULL), pUserData(NULL),
	m_uvLoop(NULL)
{
	// TODO Auto-generated constructor stub

}

TCPServer::~TCPServer() {
	// TODO Auto-generated destructor stub
}

void TCPServer::SetNetServerInfo(int32_t nPort, const char* szIpAddress)
{
    m_nPort = nPort;
    m_strLocalIp = szIpAddress;
}

 void TCPServer::connection_cb(uv_stream_t* server, int status)
 {
   TCPServer* pServer = (TCPServer*)server->data;
   pServer->processConnection(server, status);
 }

void TCPServer::SetConnectCallback(CONNECTIION_CALLBACK connectionCb, void* userData)
{
     pConnectionCb = connectionCb;
     pUserData = userData;
}

void TCPServer::after_alloc(uv_handle_t* handle,
                         size_t suggested_size,
                         uv_buf_t* buf)
{
     STREAM_PARAM* pStreamParam = (STREAM_PARAM*)handle->data;
    if (suggested_size > pStreamParam->bufferLen)
    {
        delete []pStreamParam->recvBuffer;
        pStreamParam->recvBuffer = new char[suggested_size];
    }

    buf->base = pStreamParam->recvBuffer;
    buf->len = suggested_size;
 }

void TCPServer::after_read(uv_stream_t* stream,
                        ssize_t nread,
                        const uv_buf_t* buf)
{

	 STREAM_PARAM* pStreamParam = (STREAM_PARAM*)stream->data;
	 if (pStreamParam->pReadCallBack)
	 {
		 pStreamParam->pReadCallBack(stream,  nread, buf);
	 }
}

void TCPServer::processConnection(uv_stream_t *server, int status)
{
     uv_stream_t* stream = (uv_stream_t*) new uv_tcp_t;
     int r = uv_tcp_init(m_uvLoop, (uv_tcp_t*)stream);
     if (r == 0){

         r = uv_accept(server, stream);
         if (r == 0 && pConnectionCb != NULL){


             STREAM_PARAM* pStreamParam = new STREAM_PARAM;
             memset(pStreamParam, 0, sizeof(STREAM_PARAM));
             stream->data = pStreamParam;
             pConnectionCb(stream, pUserData);
             r = uv_read_start(stream, after_alloc, after_read);
         }

     }

}

int TCPServer::InitNetServer()
{
    struct sockaddr_in addr;
    int r;

    m_uvLoop = uv_loop_new();
    if (m_strLocalIp == "")
    {
        uv_ip4_addr("127.0.0.1", m_nPort, &addr);
    }
    else
    {
        uv_ip4_addr(m_strLocalIp.c_str(),m_nPort, &addr);
    }

    r = uv_tcp_init(m_uvLoop, &m_uvTcp);
    if (r != 0)
    {
        return r;
    }

    r = uv_tcp_bind(&m_uvTcp, (sockaddr*)&addr, 0);
    if (r != 0)
    {
        return r;
    }

    m_uvTcp.data = this;
    r = uv_listen((uv_stream_t*)&m_uvTcp, 128, connection_cb);
    if (r != 0)
    {
        return r;
    }

    uv_run(m_uvLoop, UV_RUN_DEFAULT);
    return 0;
}
