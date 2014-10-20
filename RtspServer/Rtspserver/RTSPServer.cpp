#include "RTSPServer.h"

char const* RTSPServer::allowedCommandNames() {
	return "OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, GET_PARAMETER, SET_PARAMETER";
}

void RTSPClientConnection::handleCmd_OPTIONS()
{
	snprintf((char*)fResponseBuffer, sizeof fResponseBuffer,
		"RTSP/1.0 200 OK\r\nCSeq: %s\r\n%sPublic: %s\r\n\r\n",
		fCurrentCSeq, dateHeader(), fOurServer.allowedCommandNames());
}

void RTSPClientConnection::handleCmd_bad()
{
	snprintf((char*)fResponseBuffer, sizeof fResponseBuffer,
		"RTSP/1.0 400 Bad Request\r\n%sAllow: %s\r\n\r\n",
		dateHeader(), fOurServer.allowedCommandNames());
}

void RTSPClientConnection::handleCmd_DESCRIBE(char const* urlPreSuffix, char const* urlSuffix, 
											  char const* fullRequestStr)
{
	char* sdpDescription = NULL;
	char* rtspURL = NULL;

	do{
		char urlTotalSuffix[RTSP_PARAM_STRING_MAX];
		if (strlen(urlPreSuffix) + strlen(urlSuffix) + 2 > sizeof urlTotalSuffix) 
		{
			handleCmd_bad();
			break;
		}

		urlTotalSuffix[0] = '\0';
		if (urlPreSuffix[0] != '\0')
		{
			strcat(urlTotalSuffix, urlPreSuffix);
			strcat(urlTotalSuffix, "/");
		}
		strcat(urlTotalSuffix, urlSuffix);
	}while (0);
}

