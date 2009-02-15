#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <boost/asio.hpp>

class Client
{
public:
	Client(boost::asio::io_service&);

	virtual void Connnected() = 0;
	virtual void MsgReceived(const std::string& msg) = 0;
	virtual void MsgSent() = 0;
	
	void Connect(const std::string& host, const unsigned hostport);
	void Send(const std::string& msg);

private:
	void _Connected(const boost::system::error_code& ec);
	void _DataRecieved(const boost::system::error_code& ec);
	void _DataSent(const boost::system::error_code& ec);

	boost::asio::ip::tcp::socket sock;
	boost::asio::streambuf data_;

};

#endif
