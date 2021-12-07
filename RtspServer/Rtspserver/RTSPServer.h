#ifndef RTSPSERVER_H_
#define RTSPSERVER_H_

#include "../include/NetCommon.h"
#include "../ServerMediaSession/ServerMediaSession.h"
#include "RTSPCommon.hh"
#include <map>
using namespace std;

#ifndef RTSP_BUFFER_SIZE
#define RTSP_BUFFER_SIZE 10000 // for incoming requests, and outgoing responses
#endif

class RTSPClientSession;
class RTSPClientConnection;
class RTSPServer	
{
public:
	static RTSPServer* createNew(u_int32_t rtspPort, const char* localIpAddress);
public:
	RTSPServer(TCPServer* tcpServer);
	virtual ~RTSPServer(){};

	// used to implement "RTSPClientConnection::handleCmd_OPTIONS()"
	 virtual char const* allowedCommandNames();

protected:
	virtual RTSPClientSession* createNewClientSession(u_int32_t sessionId);

private:
	static void  incomingConnectionHandlerRTSPEx(uv_stream_t* stream, void* userData);
	void incomingConnectionHandlerRTSPEx(uv_stream_t* stream);

	static void incomingConnectionHandlerRTSP(void*, int /*mask*/);
	void incomingConnectionHandlerRTSP1();

	void incomingConnectionHandler(uv_stream_t* stream);

private:
	TCPServer*	pTcpServer;
	
	std::map<RTSPClientConnection*, RTSPClientConnection*> connectionMap;
protected:
	friend class RTSPClientConnection;
};
//dddfffffffffffffffffffffff1
class RTSPClientConnection
{
public:
	RTSPClientConnection(RTSPServer& ourServer, int clientSocket, struct sockaddr_in clientAddr);
	RTSPClientConnection(RTSPServer& ourServer, uv_stream_t* stream);

	virtual ~RTSPClientConnection(){};

protected:
	friend class RTSPClientSession;
	// Make the handler functions for each command virtual, to allow subclasses to reimplement them, if necessary:
	virtual void handleCmd_OPTIONS();
	// You probably won't need to subclass/reimplement this function; reimplement "RTSPServer::allowedCommandNames()" instead.
	virtual void handleCmd_GET_PARAMETER(char const* fullRequestStr); // when operating on the entire server
	virtual void handleCmd_SET_PARAMETER(char const* fullRequestStr); // when operating on the entire server
	virtual void handleCmd_DESCRIBE(char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr);
	virtual void handleCmd_REGISTER(char const* url, char const* urlSuffix, char const* fullRequestStr,
		Boolean reuseConnection, Boolean deliverViaTCP, char const* proxyURLSuffix);
	// You probably won't need to subclass/reimplement this function;
	//     reimplement "RTSPServer::weImplementREGISTER()" and "RTSPServer::implementCmd_REGISTER()" instead.
	virtual void handleCmd_bad();
	virtual void handleCmd_notSupported();
	virtual void handleCmd_notFound();
	virtual void handleCmd_sessionNotFound();
	virtual void handleCmd_unsupportedTransport();
	// Support for optional RTSP-over-HTTP tunneling:
	virtual Boolean parseHTTPRequestString(char* resultCmdName, unsigned resultCmdNameMaxSize,
		char* urlSuffix, unsigned urlSuffixMaxSize,
		char* sessionCookie, unsigned sessionCookieMaxSize,
		char* acceptStr, unsigned acceptStrMaxSize);
	virtual void handleHTTPCmd_notSupported();
	virtual void handleHTTPCmd_notFound();
	virtual void handleHTTPCmd_OPTIONS();
	virtual void handleHTTPCmd_TunnelingGET(char const* sessionCookie);
	virtual Boolean handleHTTPCmd_TunnelingPOST(char const* sessionCookie, unsigned char const* extraData, 
		unsigned extraDataSize);
	virtual void handleHTTPCmd_StreamingGET(char const* urlSuffix, char const* fullRequestStr);

public:
	void handleRequestBytes(int newBytesRead);

	void resetRequestBuffer();

protected:
	friend class RTSPServer;
	RTSPServer& fOurServer;
	uv_stream_t* socketStream;

	unsigned char fResponseBuffer[RTSP_BUFFER_SIZE];
    unsigned char fRequestBuffer[RTSP_BUFFER_SIZE];

	unsigned fRequestBytesAlreadySeen, fRequestBufferBytesLeft;

	char const* fCurrentCSeq;

	unsigned fRecursionCount;

	Boolean fIsActive;

	unsigned char* fLastCRLF;

	int fClientInputSocket, fClientOutputSocket;

	unsigned fBase64RemainderCount; // used for optional RTSP-over-HTTP tunneling (possible values: 0,1,2,3)
};

class RTSPClientSession 
{
public:
	RTSPClientSession(RTSPServer& ourServer, u_int32_t sessionId);
	virtual ~RTSPClientSession();

protected:
	friend class RTSPServer;
	friend class RTSPClientConnection;

	// Make the handler functions for each command virtual, to allow subclasses to redefine them:
	virtual void handleCmd_SETUP(RTSPClientConnection* ourClientConnection,
		char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr);
	virtual void handleCmd_withinSession(RTSPClientConnection* ourClientConnection,
		char const* cmdName,	
		char const* urlPreSuffix, char const* urlSuffix,
		char const* fullRequestStr);
	virtual void handleCmd_TEARDOWN(RTSPClientConnection* ourClientConnection,
		ServerMediaSubsession* subsession);
	virtual void handleCmd_PLAY(RTSPClientConnection* ourClientConnection,
		ServerMediaSubsession* subsession, char const* fullRequestStr);
	virtual void handleCmd_PAUSE(RTSPClientConnection* ourClientConnection,
		ServerMediaSubsession* subsession);
	virtual void handleCmd_GET_PARAMETER(RTSPClientConnection* ourClientConnection,
		ServerMediaSubsession* subsession, char const* fullRequestStr);
	virtual void handleCmd_SET_PARAMETER(RTSPClientConnection* ourClientConnection,
		ServerMediaSubsession* subsession, char const* fullRequestStr);

public:
	Boolean fIsMulticast, fStreamAfterSETUP;
	
	ServerMediaSession* fOurServerMediaSession;
};

#endif
