#ifndef PROTOCOL_HANDLER 
#define PROTOCOL_HANDLER


#include "Client.h"


class ProtocolHandler : public Client
{
public:
	typedef std::vector<std::string> ArgVec;
	ProtocolHandler(boost::asio::io_service&);
	~ProtocolHandler();
	
	void Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName);
	void SendMessage(const std::string& command, const ArgVec& arguments, unsigned msgId = 0);
	
	virtual void MessageRecieved(unsigned msgId, const std::string& command, const ArgVec& arguments) {};
	
private:
	virtual void Connnected();
	virtual void MsgReceived(const std::string& msg);
	virtual void MsgSent(const std::string& msg);

	std::string username, passwdhash;
};

#endif
