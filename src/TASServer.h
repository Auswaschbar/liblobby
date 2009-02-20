#ifndef TASSERVER_H
#define TASSERVER_H

#include "ProtocolHandler.h"


class TASServer : public ProtocolHandler
{
public:
	TASServer();
	
	//TODO: offerfile / requestupdate, Battle handling
	virtual void TASServerMsg(const std::string& springVersion, const std::string& ServerVersion, int UDPPort, int mode) {};
	virtual void ServerMessage(const std::string& message) {};
	virtual void ServerMessageBox(const std::string& message, const std::string& url) {};
	
	void RegisterAccount(const std::string& user, const std::string& passwd);
	virtual void RegisterSuccess() {};
	virtual void RegisterFail(const std::string& reason) {};
	
	void RenameAccount(const std::string& newname);
	void ChangePasword(const std::string& oldpass, const std::string& newpass);
	
	virtual void Agreement(const std::string&) {};
	virtual void AgreementEnd() {};
	void AgreementConfirm();

	void Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName);
	virtual void LoginSuccess(const std::string& username) {};
	virtual void LoginFail(const std::string& reason) {};
	virtual void LoginInfoEnd() {};

	virtual void Motd(const std::string& message) {};

	void Join(const std::string& channel, const std::string& key = "");
	virtual void JoinSuccess(const std::string& channame) {};
	virtual void JoinFail(const std::string& channame, const std::string& reason) {};
	
	void Leave(const std::string& channel);
	
	void Say(const std::string& channame, const std::string& message);
	virtual void Said(const std::string& channame, const std::string& username, const std::string& message) {};
	
	void SayEx(const std::string& channame, const std::string& message);
	virtual void SaidEx(const std::string& channame, const std::string& username, const std::string& message) {};
	
	void SayPrivate(const std::string& username, const std::string& message);
	virtual void MeSaidPrivate(const std::string& username, const std::string& message) {};
	virtual void SaidPrivate(const std::string& username, const std::string& message) {};
	
	void Ping();
	virtual void Pong() {};
	
	virtual void AddUser(const std::string& username, const std::string& country, const std::string& cpu) {};
	virtual void RemoveUser(const std::string& username) {};
	void MyStatus(bool ingame, bool away);
	virtual void ClientStatus(const std::string& name, bool ingame, bool away, int rank, bool moderator, bool bot) {};

protected:
	virtual void MessageRecieved(const InMessage& msgs);
};

#endif
