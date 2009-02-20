#include "ClanBot.h"

#include <fstream>
#include <sstream>

using namespace std;

ClanBot::ClanBot(const std::string& _server, int _port, const std::string& user_, const std::string& passwd_) : server(_server), port(_port), user(user_), passwd(passwd_)
{
	Connect(server, port);
	
	fstream channelsFile("channels.txt");
	while (channelsFile.is_open() && !channelsFile.eof())
	{
		std::string line;
		std::getline(channelsFile,line);
		std::istringstream lineBuffer(line);
		std::string channel;
		lineBuffer >> channel;
		if (!channel.empty())
		{
			channels.push_back(channel);
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
	for (channelList::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Join(*it);
	}
}

void ClanBot::LoginFail(const std::string& reason)
{
	cout << "Login failed: " << reason;
}

void ClanBot::Said(const std::string& channame, const std::string& username, const std::string& message)
{
	std::istringstream stringbuf(HandleMessage(username, message));
	while (!stringbuf.eof())
	{
		std::string line;
		std::getline(stringbuf,line);
		SayEx(channame, line);
	}
}

void ClanBot::SaidPrivate(const std::string& username, const std::string& message)
{
	std::istringstream stringbuf(HandleMessage(username, message));
	while (!stringbuf.eof())
	{
		std::string line;
		std::getline(stringbuf,line);
		SayPrivate(username, line);
	}
}

std::string ClanBot::HandleMessage(const std::string& username, const std::string& message)
{
	if (message.find("!online") == 0)
	{
		size_t minOnline = 2;
		if (message.size() > 8)
		{
			std::istringstream stringbuf(message);
			std::string temp;
			stringbuf >> temp;
			stringbuf >> minOnline;
		}
		
		ostringstream buf;
		buf << "Clan Players online:\n";
		for (clanMap::const_iterator clanIt = clanUserMap.begin(); clanIt != clanUserMap.end(); ++clanIt)
		{
			if (clanIt->second.size() >= minOnline)
			{
				buf << clanIt->first;
				for (size_t i = clanIt->first.size(); i <= 10; ++i)
					buf << " ";
				buf << clanIt->second.size() << endl;
				/*for (std::list <User>::const_iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
				{
					buf << "# " << it->name;
					for (size_t i = it->name.size(); i <= 30; ++i)
						buf << " ";
					buf << it->country << " " << it->cpu << endl;
				}*/
			}
		}
		return buf.str();
	}
	else if (message.find("!clan") == 0)
	{
		if (message.size () <= 6)
			return "";
		
		std::istringstream stringbuf(message);
		std::string tempclan;
		stringbuf >> tempclan;
		stringbuf >> tempclan;
		
		clanMap::const_iterator clanIt = clanUserMap.find(tempclan);
		ostringstream buf;
		
		if (clanIt != clanUserMap.end())
		{
			buf << "Online players of clan " << tempclan << ": " << clanIt->second.size() << endl;
			for (std::list <User>::const_iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
			{
				buf << "# " << it->name;
				for (size_t i = it->name.size(); i <= 30; ++i)
					buf << " ";
				buf << it->country << " " << it->cpu << endl;
			}
		}
		else
		{
			buf << "Clan " << tempclan << " deos not exists or there are no members online" << endl;
		}
		return buf.str();
	}
	else if (message.find("!addchannel") == 0)
	{
		std::istringstream stringbuf(message);
		std::string buf;
		stringbuf >> buf;
		stringbuf >> buf;
		Join(buf);
		channels.push_back(buf);
		ostringstream output;
		output << "Joining channel " << buf << endl;
		return output.str();
	}
	else
		return "";
}

std::string GetClanFromName(const std::string& username)
{
	size_t first = username.find_first_of('[');
	if (first != string::npos)
	{
		size_t next = username.find_first_of(']', first+1);
		const std::string clan = username.substr(first+1, next - first-1);
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
