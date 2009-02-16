#include "ProtocolHandler.h"

#include <iostream>
#include <sstream>

#include "Client.h"


ProtocolHandler::ProtocolHandler()
{
}

ProtocolHandler::~ProtocolHandler()
{
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

