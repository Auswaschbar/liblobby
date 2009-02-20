#include "ClanBot.h"

#include <fstream>
#include <sstream>

using namespace std;

ClanBot::ClanBot(const std::string& _server, int _port, const std::string& user_, const std::string& passwd_) : server(_server), port(_port), user(user_), passwd(passwd_)
{
	Connect(server, port);
	
	fstream channels("channels.txt");
	while (!channels.eof())
	{
		std::string line;
		std::getline(channels,line);
		std::istringstream lineBuffer(line);
		std::string clan;
		lineBuffer >> clan;
		while (lineBuffer.eof())
		{
			std::string channel;
			lineBuffer >> channel;
			if (!channel.empty()) // safety
				clanChannels[clan].push_back(channel);
		}
	}
}

void ClanBot::Connected()
{
	Login(user, passwd, "1337", "*", "liblobby V0.2");
}

void ClanBot::Disconnnected()
{
	Connect(server, port);
}

void ClanBot::LoginSuccess(const std::string& username)
{
	Join("clanbot");
	for (clanChannelLists::const_iterator it = clanChannels.begin(); it != clanChannels.end(); ++it)
	{
		for (channelList::const_iterator channel = it->second.begin(); channel != it->second.end(); ++channel)
		{
			Join(*channel);
		}
	}
}

void ClanBot::LoginFail(const std::string& reason)
{
	cout << "Login failed: " << reason;
}

void ClanBot::Said(const std::string& channame, const std::string& username, const std::string& message)
{
	SayEx(channame, HandleMessage(username, message));
}

void ClanBot::SaidPrivate(const std::string& username, const std::string& message)
{
	SayPrivate(username, HandleMessage(username, message));
}

std::string ClanBot::HandleMessage(const std::string& username, const std::string& message)
{
	if (message.find("!online") == 0)
	{
		ostringstream buf;
		for (clanMap::const_iterator clanIt = clanUserMap.begin(); clanIt != clanUserMap.end(); ++clanIt)
		{
			buf << clanIt->first << std::endl;
			for (std::list <User>::const_iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
			{
				
				buf << "# " << it->name;
				for (size_t i = it->name.size(); i >= 20; ++i)
					buf << " ";
				buf << it->country << " " << it->cpu << endl;
			}
		}
	}
}

std::string GetClanFromName(const std::string& username)
{
	size_t first = username.find_first_of('[');
	if (first != string::npos)
	{
		size_t next = username.find_first_of(']', first+1);
		const std::string clan = username.substr(first, next - first);
		return clan;
	}
	else
		return "";
}

void ClanBot::AddUser(const std::string& username, const std::string& country, const std::string& cpu)
{
	const std::string clan = GetClanFromName(username);
	if (!clan.empty())
	{
		User temp;
		temp.name = username;
		temp.country = country;
		temp.cpu = cpu;
		clanUserMap[clan].push_back(temp);
	}
}

void ClanBot::RemoveUser(const std::string& username)
{
	const std::string clan = GetClanFromName(username);
	if (!clan.empty())
	{
		clanMap::iterator clanIt = clanUserMap.find(clan);
		if (clanIt != clanUserMap.end())
		{
			for (std::list <User>::iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
			{
				if (it->name == username)
				{
					clanIt->second.erase(it);
					return;
				}
			}
		}
	}
}
