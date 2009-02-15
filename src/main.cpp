#include "ProtocolHandler.h"


int main()
{
	boost::asio::io_service io_service;

	ProtocolHandler client(io_service);
	client.Connect("82.239.138.44", 8300);
	client.Login("Test", "lolpasswd", "1337", "*", "liblobby V0.1");
	io_service.run();

	return 0;
};
