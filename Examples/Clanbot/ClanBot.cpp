#include "ClanBot.h"

#include <fstream>
#include <sstream>

using namespace std;

ClanBot::ClanBot(const std::string& _server, int _port, const std::string& user_, const std::string& passwd_) : server(_server), port(_port), user(user_), passwd(passwd_)
{
	Connect(server, port);
	
	fstream channelsFile("channels.list");
	channels.insert("clanbot");
	while (channelsFile.is_open() && !channelsFile.eof())
	{
		std::string line;
		std::getline(channelsFile,line);
		std::istringstream lineBuffer(line);
		std::string channel;
		lineBuffer >> channel;
		if (!channel.empty())
		{
			channels.insert(channel);
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
	for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
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
	if (message.find("!gtfo") == 0)
	{
		Leave(channame);
		for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if (*it == channame)
			{
				channels.erase(it);
				break;
			}
		}
	}
	else
	{
		std::istringstream stringbuf(HandleMessage(username, message));
		while (!stringbuf.eof())
		{
			std::string line;
			std::getline(stringbuf,line);
			if (!line.empty())
				SayEx(channame, line);
		}
	}
}

void ClanBot::SaidPrivate(const std::string& username, const std::string& message)
{
	std::istringstream stringbuf(HandleMessage(username, message));
	while (!stringbuf.eof())
	{
		std::string line;
		std::getline(stringbuf,line);
		if (!line.empty())
			SayPrivate(username, line);
	}
}

void ClanBot::JoinSuccess(const std::string& channame)
{
	if (channels.count(channame) == 0)
	{
		channels.insert(channame);
		std::ofstream file("channels.list");
		for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
		{
			file << *it << endl;
		}
	}
}

void ClanBot::JoinFail(const std::string& channame, const std::string& reason)
{
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
			unsigned numOnline = 0;
			for (std::list <User>::const_iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
			{
				if (!it->isBot)
					++numOnline;
			}
			if (numOnline >= minOnline)
			{
				buf << clanIt->first;
				for (size_t i = clanIt->first.size(); i <= 10; ++i)
					buf << " ";
				buf << numOnline << endl;
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
			buf << "Online players of clan " << tempclan << ": " << CountPlayersOnlineClan(tempclan) << endl;
			for (std::list <User>::const_iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
			{
				if (!it->isBot)
				{
					buf << "# " << it->name;
					for (size_t i = it->name.size(); i <= 30; ++i)
						buf << " ";
					buf << it->country << " " << it->cpu << endl;
				}
			}
		}
		else
		{
			buf << "Clan " << tempclan << " does not exists or there are no members online" << endl;
		}
		return buf.str();
	}
	else if (message.find("!addchannel") == 0)
	{
		std::istringstream stringbuf(message);
		std::string buf;
		stringbuf >> buf;
		stringbuf >> buf;
		ostringstream output;
		if (channels.count(buf) == 0)
		{
			Join(buf);
			output << "Joining channel " << buf << endl;
		}
		else
			output << "Already in channel " << buf << endl;
		return output.str();
	}
	else
		return "";
}

unsigned ClanBot::CountPlayersOnlineClan(const std::string& clanname)
{
	clanMap::const_iterator clanIt = clanUserMap.find(clanname);
	if (clanIt != clanUserMap.end())
	{
		unsigned num = 0;
		for (std::list <User>::const_iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
		{
			if (!it->isBot)
				++num;
		}
		return num;
	}
	else
		return 0;
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
		temp.isBot = false;
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

void ClanBot::ClientStatus(const std::string& username, bool ingame, bool away, int rank, bool moderator, bool bot)
{
	if (bot)
	{
		clanMap::iterator clanIt = clanUserMap.find(GetClanFromName(username));
		if (clanIt != clanUserMap.end())
		{
			for (std::list <User>::iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
			{
				if (it->name == username)
				{
					it->isBot = true;
					return;
				}
			}
		}
	}
}