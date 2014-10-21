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

void RTSPClientConnection::handleRequestBytes(int newBytesRead) 
{
	int numBytesRemaining = 0;
	++fRecursionCount;

	do 
	{
		RTSPClientSession* clientSession = NULL;
		if (newBytesRead < 0 || (unsigned)newBytesRead > fRequestBufferBytesLeft)
		{
#ifdef DEBUG
			fprintf(stderr, "RTSPClientConnection[%p]::handleRequestBytes() read %d new bytes (of %d); terminating connection!\n", this, newBytesRead, fRequestBufferBytesLeft);
#endif
			fIsActive = False;
			break;
		}

		Boolean endOfMsg = False;
		unsigned char* ptr = &fRequestBuffer[fRequestBytesAlreadySeen];
#ifdef DEBUG
		ptr[newBytesRead] = '\0';
		fprintf(stderr, "RTSPClientConnection[%p]::handleRequestBytes() %s %d new bytes:%s\n",
			this, numBytesRemaining > 0 ? "processing" : "read", newBytesRead, ptr);
#endif
	unsigned char* tmpPtr = fLastCRLF + 2;
	if (tmpPtr < fRequestBuffer)
	{
		tmpPtr = fRequestBuffer;
	}

	//²éÕÒ»»ÐÐ·û.
	while (tmpPtr < &ptr[newBytesRead - 1])
	{
		if (*tmpPtr == '\r' && *(tmpPtr + 1) == '\n')
		{
			if (tmpPtr - fLastCRLF == 2)
			{
				endOfMsg = True;
				break;
			}

			fLastCRLF = tmpPtr;
		}

		++tmpPtr;
	}

	fRequestBufferBytesLeft -= newBytesRead;
	fRequestBytesAlreadySeen += newBytesRead;

	if (!endOfMsg) break; // subsequent reads will be needed to complete the request

	fRequestBuffer[fRequestBytesAlreadySeen] = '\0';
	char cmdName[RTSP_PARAM_STRING_MAX];
	char urlPreSuffix[RTSP_PARAM_STRING_MAX];
	char urlSuffix[RTSP_PARAM_STRING_MAX];
	char cseq[RTSP_PARAM_STRING_MAX];
	char sessionIdStr[RTSP_PARAM_STRING_MAX];
	unsigned contentLength = 0;
	fLastCRLF[2] = '\0'; // temporarily, for parsing
	
	Boolean parseSucceed = parseRTSPRequestString((char*)fRequestBuffer, fLastCRLF + 2 - fRequestBuffer, 
						cmdName, sizeof cmdName,
						urlPreSuffix, sizeof urlPreSuffix,
						urlSuffix, sizeof urlSuffix,
						cseq, sizeof cseq,
						sessionIdStr, sizeof sessionIdStr,
						contentLength);

	fLastCRLF[2] = '\r'; // restore its value
	Boolean playAfterSetup = False;
	
	if (parseSucceed)
	{
#ifdef DEBUG
		fprintf(stderr, "parseRTSPRequestString() succeeded, returning cmdName \"%s\", urlPreSuffix \"%s\", urlSuffix \"%s\", CSeq \"%s\", Content-Length %u, with %ld bytes following the message.\n", cmdName, urlPreSuffix, urlSuffix, cseq, contentLength, ptr + newBytesRead - (tmpPtr + 2));
#endif
	

	}

	} while (1);
	
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

