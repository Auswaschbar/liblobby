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
	loggedIn = false;
};

void TASServer::Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName)
{
	ArgVec args;
	args.push_back(user);
	boost::md5 checksum(passwd.c_str());
	args.push_back(checksum.digest().hex_str_value());
	args.push_back(cpu);
	args.push_back(localIP);
	args.push_back(lobbyName);
	SendMessage(Message("LOGIN", args));
}

void TASServer::MessageRecieved(const Message& msg)
{
	if (msg.GetCommand() == "TASServer")
	{
		serverVersion = msg.GetWord();
		springVersion = msg.GetWord();
		StringConvert(msg.GetWord(), UDPPort);
		StringConvert(msg.GetWord(), servermode);
	}
	else if (msg.GetCommand() == "ACCEPTED")
	{
		std::cout << "Login accepted" << std::endl;
		loggedIn = true;
	}
	else if (msg.GetCommand() == "MOTD")
	{
	}
}
