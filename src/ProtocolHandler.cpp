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

void ProtocolHandler::SendMessage(const Message& msg)
{
	Send(msg.GetFullStr());
}

void ProtocolHandler::Connnected()
{
	std::cout << "Client connected" << std::endl;
}

void ProtocolHandler::MsgReceived(const std::string& msg)
{
	MessageRecieved(InMessage(msg));
}

void ProtocolHandler::MsgSent(const std::string& msg)
{
	//std::cout << "Data sent" << std::endl;
}

