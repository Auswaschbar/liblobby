#include "Client.h"

#include <sstream>
#include <boost/bind.hpp>
#include <iostream>

using namespace std;

Client::Client()
{
	sock.reset(new boost::asio::ip::tcp::socket(io_service));
	std::cout << "Client constructed" << std::endl;
	connected = false;
}

Client::~Client()
{
	Disconnect();
}

void Client::Connect(const std::string& host, const unsigned hostport)
{
	std::cout << "Connecting to " << host << ":" << hostport << std::endl;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), hostport);
	sock->async_connect(endpoint, boost::bind(&Client::_Connected, this, _1) );
	netThread = boost::thread(boost::bind(&Client::Run, this));
}

void Client::Disconnect()
{
	io_service.stop();
	netThread.join();
	sock->close();
	connected = false;
}

void Client::Send(const std::string& msg)
{
	std::cout << "<< " << msg << std::endl;
	sock->async_send(boost::asio::buffer(msg), boost::bind(&Client::_DataSent, this, _1, msg));
}

void Client::Run()
{
	std::cout << "Running seperate thread" << std::endl;
	io_service.run();
	std::cout << "Finished running seperate thread" << std::endl;
}

void Client::_Connected(const boost::system::error_code& ec)
{
	if (!ec)
	{
		boost::asio::async_read_until(*sock, data_, "\n", boost::bind(&Client::_DataRecieved, this, _1));
		connected = true;
		Connnected();
	}
	else
	{
		std::cout << "Connecting failed: " << ec << std::endl;
	}
}

void Client::_DataRecieved(const boost::system::error_code& ec)
{
	if (!ec)
	{
		std::string msg;
		std::istream buf(&data_);
		std::getline(buf, msg);
		boost::asio::async_read_until(*sock, data_, "\n", boost::bind(&Client::_DataRecieved, this, _1));
		
		std::cout << ">> " << msg << std::endl;
		MsgReceived(msg);
	}
	else if (ec.value() == boost::asio::error::eof)
	{
		std::cout << "Client disconnected" << std::endl;
	}
	else
	{
		std::cout << "Error in DataRecieved: " << ec << std::endl;
	}
}

void Client::_DataSent(const boost::system::error_code& ec, const std::string& msg)
{
	if (!ec)
	{
		MsgSent(msg);
	}
	else
	{
		std::cout << "Error in DataSent: " << ec.message() << std::endl;
	}
}
