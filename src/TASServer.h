#ifndef TASSERVER_H
#define TASSERVER_H

#include "ProtocolHandler.h"


class TASServer : public ProtocolHandler
{
public:
	TASServer();
	
	void RegisterAccount(const std::string& user, const std::string& passwd);
	virtual void RegisterSuccess() {};
	virtual void RegisterFail(const std::string& reason) {};
	
	virtual void Agreement(const std::string&) {};
	virtual void AgreementEnd() {};
	void AgreementConfirm();

	void Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName);
	virtual void LoginSuccess(const std::string& username) {};
	virtual void LoginFail(const std::string& reason) {};

	void Join(const std::string& channel, const std::string& key = "");
	virtual void JoinSuccess(const std::string& channame) {};
	virtual void JoinFail(const std::string& channame, const std::string& reason) {};
	
	void Leave(const std::string& channel);
	
	void Say(const std::string& channame, const std::string& message);
	virtual void Said(const std::string& channame, const std::string& username, const std::string& message) {};
	
	void Ping();
	virtual void Pong() {};

protected:
	virtual void MessageRecieved(const InMessage& msgs);
};

#endif
