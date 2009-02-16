#include "TASServer.h"

#include <sstream>

#include "md5.hpp"

template<typename T>
void StringConvert(const std::string& input, T& t)
{
	std::istringstream stream(input);
	stream >> t;
}

void TASServer::Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName)
{
	ArgVec args;
	args.push_back(user);
	boost::md5 checksum(passwd.c_str());
	args.push_back(checksum.digest().hex_str_value());
	args.push_back(cpu);
	args.push_back(localIP);
	args.push_back(lobbyName);
	SendMessage("LOGIN", args);
}

void TASServer::MessageRecieved(unsigned msgId, const std::string& command, const ArgVec& arguments)
{
	assert(!arguments.empty());
	if (arguments[0] == "TASServer")
	{
		serverVersion = arguments[0];
		springVersion = arguments[1];
		StringConvert(arguments[2], UDPPort);
		StringConvert(arguments[3], servermode);
	}
	else if (arguments[0] == "ACCEPTED")
	{
		std::cout << "Login accepted" << std::endl;
		loggedIn = true;
	}
}