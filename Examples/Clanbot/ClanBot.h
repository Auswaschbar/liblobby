#ifndef CLANBOT_H
#define CLANBOT_H

#include "TASServer.h"

#include <map>
#include <list>

struct User
{
	std::string name;
	std::string country;
	std::string cpu;
};

class ClanBot : public TASServer
{
public:
	ClanBot(const std::string& server, int port, const std::string& user, const std::string& passwd);
	
private:
	// TASServer callbacks
	virtual void Connected();
	virtual void Disconnnected();

	virtual void LoginSuccess(const std::string& username);
	virtual void LoginFail(const std::string& reason);

	virtual void Said(const std::string& channame, const std::string& username, const std::string& message);
	virtual void SaidPrivate(const std::string& username, const std::string& message);
	
	virtual void AddUser(const std::string& username, const std::string& country, const std::string& cpu);
	virtual void RemoveUser(const std::string& username);

	// own functions
	std::string HandleMessage(const std::string& username, const std::string& message);

	typedef std::list<std::string> channelList;
	channelList channels;
	
	typedef std::map< std::string, std::list <User> > clanMap;
	clanMap clanUserMap;
	
	const std::string server;
	int port;
	const std::string user, passwd;
};

#endif
