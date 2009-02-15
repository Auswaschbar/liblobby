#include "ProtocolHandler.h"

#include <iostream>
#include <sstream>
#include <boost/format.hpp>

#include "Client.h"
#include "md5.hpp"

using boost::format;

ProtocolHandler::ProtocolHandler(boost::asio::io_service& service) : Client(service)
{
	// netThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
}

ProtocolHandler::~ProtocolHandler()
{
}

void ProtocolHandler::Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName)
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

void ProtocolHandler::SendMessage(const std::string& command, const ArgVec& arguments, unsigned msgId)
{
	std::ostringstream out;
	if (msgId > 0)
	{
		out << '#' << msgId << ' ';
	}
	out << command;
	for (ArgVec::const_iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		out << ' ' << *it;
	}
	out << std::endl;
	Send(out.str());
}

void ProtocolHandler::Connnected()
{
	std::cout << "Client connected" << std::endl;
}

void ProtocolHandler::MsgReceived(const std::string& msg)
{
	unsigned commandId = 0;
	std::istringstream inbuf(msg);
	if (msg.at(0) == '#')
	{
		inbuf.seekg(1, std::ios::beg);
		inbuf >> commandId;
	}
	std::string command;
	inbuf >> command;

	std::vector<std::string> params;
	while (!inbuf.eof())
	{
		std::string bufstr;
		inbuf >> bufstr;
		params.push_back(bufstr);
	}
	MessageRecieved(commandId, command, params);
}

void ProtocolHandler::MsgSent(const std::string& msg)
{
	//std::cout << "Data sent" << std::endl;
}
/*
void ProtocolHandler::HandleMessage(const std::vector<std::string>& words)
{
	assert(!words.empty());
	if (words[0] == "TASServer")
	{
		
	}
	else if (words[0] == "ACCEPTED")
	{
		std::cout << "Login accepted" << std::endl;
	}
}*/
