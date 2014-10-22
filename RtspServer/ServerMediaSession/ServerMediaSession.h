#pragma once

class ServerMediaSession
{
public:
	ServerMediaSession(void);
	virtual ~ServerMediaSession(void);

public:
	  char const* streamName() const { return fStreamName; }

private:
	  char* fStreamName;
};

class ServerMediaSubsession
{
	ServerMediaSubsession();
	virtual ~ServerMediaSubsession();
};