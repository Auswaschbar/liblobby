#ifndef PROTOCOL_HANDLER 
#define PROTOCOL_HANDLER


#include "Client.h"
#include "Message.h"

class ProtocolHandler : public Client
{
public:
	ProtocolHandler();
	~ProtocolHandler();

	void SendMessage(const Message&);
	virtual void MessageRecieved(const Message&) {};
	
private:
	virtual void Connnected();
	virtual void MsgReceived(const std::string& msg);
	virtual void MsgSent(const std::string& msg);

	std::string username, passwdhash;
};

#endif
