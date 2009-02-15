#include "ProtocolHandler.h"

#include <iostream>
#include <sstream>
#include <boost/format.hpp>

#include "Client.h"
#include "md5.hpp"

using boost::format;

ProtocolHandler::ProtocolHandler(boost::asio::io_service& service) : Client(service)
{
}

void ProtocolHandler::Login(const std::string& user, const std::string& passwd)
{
	boost::md5 checksum(passwd.c_str());
	passwdhash = checksum.digest().hex_str_value();
	username = user;
}

void ProtocolHandler::Connnected()
{
	std::cout << "Client connected" << std::endl;
	if (!username.empty())
		Send( str( format("LOGIN %s %s %i %s %s\t%s\n\r") %username %passwdhash %1337 %"*" %"Überlobby" %"" ) );
}

void ProtocolHandler::MsgReceived(const std::string& msg)
{
	//std::cout << msg << std::endl;
}

void ProtocolHandler::MsgSent()
{
	//std::cout << "Data sent" << std::endl;
}
