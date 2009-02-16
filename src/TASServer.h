#ifndef TASSERVER_H
#define TASSERVER_H

#include "ProtocolHandler.h"

class TASServer : public ProtocolHandler
{
public:
	TASServer()
	{
		loggedIn = false;
	};
	void Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName);
	
protected:
	virtual void MessageRecieved(unsigned msgId, const std::string& command, const ArgVec& arguments);
	
	bool loggedIn;
	std::string serverVersion;
	std::string springVersion;
	int UDPPort;
	
	static const int MODE_NORMAL = 0;
	static const int MODE_LAN = 1;
	
	int servermode;
};

#endif
