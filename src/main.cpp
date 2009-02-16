#include "TASServer.h"


int main()
{
	TASServer client;
	client.Connect("82.239.138.44", 8300);
	client.Login("Test", "lolpasswd", "1337", "*", "liblobby V0.1");
	sleep(5);
	return 0;
};
