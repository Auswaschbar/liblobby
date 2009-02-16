#include "TASServer.h"

#include <sstream>

#include "md5.hpp"

template<typename T>
void StringConvert(const std::string& input, T& t)
{
	std::istringstream stream(input);
	stream >> t;
}

TASServer::TASServer()
{
};

void TASServer::Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName)
{
	Message msg("LOGIN");
	ArgVec args;
	msg.Push(user);
	boost::md5 checksum(passwd.c_str());
	msg.Push(checksum.digest().hex_str_value());
	msg.Push(cpu);
	msg.Push(localIP);
	msg.Push(lobbyName);
	SendMessage(msg);
}

void TASServer::Join(const std::string& channel, const std::string& key)
{
	Message msg("JOIN");
	msg.Push(channel);
	SendMessage(msg);
}

void TASServer::Leave(const std::string& channel)
{
	Message msg("LEAVE");
	msg.Push(channel);
	SendMessage(msg);
}

void TASServer::Say(const std::string& channame, const std::string& message)
{
	Message msg("SAY");
	msg.Push(channame);
	msg.Push(message);
	SendMessage(msg);
}

void TASServer::MessageRecieved(const InMessage& msg)
{
	if (msg.GetCommand() == "TASServer")
	{
		/*serverVersion = msg.GetWord();
		springVersion = msg.GetWord();
		StringConvert(msg.GetWord(), UDPPort);
		StringConvert(msg.GetWord(), servermode);*/
	}
	else if (msg.GetCommand() == "ACCEPTED")
	{
		LoginSuccess(msg.GetWord());
	}
	else if (msg.GetCommand() == "DENIED")
	{
		LoginFail(msg.GetWord());
	}
	else if (msg.GetCommand() == "JOIN")
	{
		JoinSuccess(msg.GetWord());
	}
	else if (msg.GetCommand() == "JOINFAILED")
	{
		JoinFail(msg.GetWord(), msg.GetSentence());
	}
	else if (msg.GetCommand() == "SAID")
	{
		const std::string channel = msg.GetWord();
		const std::string user = msg.GetWord();
		Said(channel, user, msg.GetSentence());
	}
	else if (msg.GetCommand() == "MOTD")
	{
	}
}
