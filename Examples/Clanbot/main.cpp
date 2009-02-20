#include "ClanBot.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "Usage: clanbot <username> <password>\n";
		return 1;
	}
	ClanBot client("taspringmaster.clan-sy.com", 8200, argv[1], argv[2]);
	while (true)
	{
		sleep(10);
		client.Ping();
	}
	return 0;
};
