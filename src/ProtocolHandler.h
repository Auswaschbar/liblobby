#ifndef PROTOCOL_HANDLER 
#define PROTOCOL_HANDLER


#include "Client.h"


class ProtocolHandler : public Client
{
public:
	ProtocolHandler(boost::asio::io_service&);
	
	void Login(const std::string& user, const std::string& passwd);
	
	
	virtual void Connnected();
	virtual void MsgReceived(const std::string& msg);
	virtual void MsgSent();
	
private:
	std::string username, passwdhash;
};

#endif
