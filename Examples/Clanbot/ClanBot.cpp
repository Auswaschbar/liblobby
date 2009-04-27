#include "ClanBot.h"

#include <fstream>
#include <sstream>

using namespace std;

ClanBot::ClanBot(const std::string& _server, int _port, const std::string& user_, const std::string& passwd_) : server(_server), port(_port), user(user_), passwd(passwd_)
{
	Connect(server, port);
}

void ClanBot::Connected()
{
	Login(user, passwd, "1337", "*", "liblobby V0.2");
	cout << "Connected to TASServer\n";
	
	fstream channelsFile("channels.list");
	{
		Channel cur;
		cur.name = "clans";
		joinRequests.insert(cur);
	}
	while (channelsFile.is_open() && !channelsFile.eof())
	{
		std::string line;
		std::getline(channelsFile,line);
		std::istringstream lineBuffer(line);
		Channel cur;
		lineBuffer >> cur.name;
		if (!cur.name.empty())
		{
			lineBuffer >> cur.pwd;
			joinRequests.insert(cur);
			cout << "Added channel " << cur.name << " to channel join list\n";
		}
	}
	clanUserMap.clear();
}

void ClanBot::Disconnected()
{
	cout << "Trying to reconnect\n";
	Connect(server, port);
}

void ClanBot::LoginSuccess(const std::string& username)
{
	for (channelSet::const_iterator it = joinRequests.begin(); it != joinRequests.end(); ++it)
	{
		Join(it->name, it->pwd);
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
		if (channame == "clans")
		{
			SayEx(channame, ": GTFO yourself, noob!");
			return;
		}
		Leave(channame);
		for (channelSet::const_iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if (it->name == channame)
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
	if (!IsInChannel(channame))
	{
		Channel cur;
		cur.name = channame;
		for (channelSet::iterator requested = joinRequests.begin(); requested != joinRequests.end(); ++requested)
		{
			if (requested->name == cur.name)
			{
				cur.pwd == requested->pwd;
				joinRequests.erase(requested);
				break;
			}
		}
		channels.insert(cur);
		std::ofstream file("channels.list");
		for (channelSet::const_iterator it = channels.begin(); it != channels.end(); ++it)
		{
			file << it->name;
			if (!it->pwd.empty())
				file << "  " << it->pwd;
			file << endl;
		}
	}
}

void ClanBot::JoinFail(const std::string& channame, const std::string& reason)
{
	for (channelSet::iterator requested = joinRequests.begin(); requested != joinRequests.end(); ++requested)
	{
		if (requested->name == channame)
		{
			joinRequests.erase(requested);
			break;
		}
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
					if (!it->away && !it->ingame)
						buf << "<#> ";
					else if (it->ingame)
						buf << "<I> ";
					else if (it->away)
						buf << "<A> ";
					buf << it->name;
					for (size_t i = it->name.size(); i <= 30; ++i)
						buf << " ";
					buf << it->country << " " << it->cpu << "MHz" << endl;
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
		Channel cur;
		stringbuf >> cur.name;
		stringbuf >> cur.name;
		ostringstream output;
		if (!IsInChannel(cur.name))
		{
			stringbuf >> cur.pwd;
			Join(cur.name, cur.pwd);
			joinRequests.insert(cur);
			output << "Joining channel " << cur.name << endl;
		}
		else
			output << "Already in channel " << cur.name << endl;
		return output.str();
	}
	else if (message.find("!help") == 0)
	{
		ostringstream output;
		output << "Available commands:" << endl;
		output << "  !addchannel <channelname> [optional:password]\n";
		output << "  !clan <clanname>\n  -> <#> - user is available\n  -> <A> - away\n  -> <I> - ingame\n";
		output << "  !online\n  !gtfo\n";
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
		temp.away = false;
		temp.ingame = false;
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
	clanMap::iterator clanIt = clanUserMap.find(GetClanFromName(username));
	if (clanIt != clanUserMap.end())
	{
		for (std::list <User>::iterator it = clanIt->second.begin(); it != clanIt->second.end(); ++it)
		{
			if (it->name == username)
			{
				it->isBot = bot;
				it->ingame = ingame;
				it->away = away;
				return;
			}
		}
	}
}

bool ClanBot::IsInChannel(const std::string& channame)
{
	for (channelSet::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->name == channame)
			return true;
	}
	return false;
}
