#include "../include/NetCommon.h"
#include "../ServerMediaSession/ServerMediaSession.h"
#include "RTSPCommon.hh"

#ifndef RTSP_BUFFER_SIZE
#define RTSP_BUFFER_SIZE 10000 // for incoming requests, and outgoing responses
#endif

class RTSPServer	
{
public:
	RTSPServer();
	virtual ~RTSPServer();

	// used to implement "RTSPClientConnection::handleCmd_OPTIONS()"
	 virtual char const* allowedCommandNames();
};

class RTSPClientConnection
{
public:
	RTSPClientConnection(RTSPServer& ourServer, int clientSocket, struct sockaddr_in clientAddr);
	virtual ~RTSPClientConnection();

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

protected:
	RTSPServer& fOurServer;

	unsigned char fResponseBuffer[RTSP_BUFFER_SIZE];

	char const* fCurrentCSeq;
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
};