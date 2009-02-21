#ifndef CLANBOT_H
#define CLANBOT_H

#include "TASServer.h"

#include <map>
#include <list>
#include <set>

struct User
{
	std::string name;
	std::string country;
	std::string cpu;
	bool isBot;
};

struct Channel
{
	std::string name;
	std::string pwd;
};

struct ChannelComparator
{
	bool operator()(const Channel& s1, const Channel& s2) const
	{
		return s1.name < s2.name;
	}
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
	virtual void ClientStatus(const std::string& name, bool ingame, bool away, int rank, bool moderator, bool bot);

	virtual void JoinSuccess(const std::string& channame);
	virtual void JoinFail(const std::string& channame, const std::string& reason);
	
	// own functions
	std::string HandleMessage(const std::string& username, const std::string& message);

	unsigned CountPlayersOnlineClan(const std::string& clanname);
	
	typedef std::map< std::string, std::list <User> > clanMap;
	clanMap clanUserMap;
	
	bool IsInChannel(const std::string& channame);
	typedef std::set<Channel, ChannelComparator> channelSet;
	channelSet channels, joinRequests;
	std::map<std::string, std::string> channelPWD;
	const std::string server;
	int port;
	const std::string user, passwd;
};

#endif
