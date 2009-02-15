#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <boost/asio.hpp>

class Client
{
public:
	Client(boost::asio::io_service&);

	void Connect(const std::string& host, const unsigned hostport);

protected:
	void Send(const std::string& msg);
	
	virtual void Connnected() = 0;
	virtual void MsgReceived(const std::string& msg) = 0;
	virtual void MsgSent(const std::string& msg) = 0;

private:
	void _Connected(const boost::system::error_code& ec);
	void _DataRecieved(const boost::system::error_code& ec);
	void _DataSent(const boost::system::error_code& ec, const std::string& msg);

	boost::asio::ip::tcp::socket sock;
	boost::asio::streambuf data_;
};

#endif
