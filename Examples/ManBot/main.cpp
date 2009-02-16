#include "TASServer.h"

class ManBot : public TASServer
{
	virtual void LoginSuccess(const std::string& username)
	{
		std::cout << "Login Success\n";
		Join("sy");
	};
	
	virtual void Said(const std::string& channame, const std::string& username, const std::string& message)
	{
		std::cout << username << " said: " << message << std::endl;
		if (channame == "sy")
		{
			if (message.find("!man") == 0 && message.size() > 6)
			{
				const std::string target = "man "+message.substr(5, message.find(" \t\n", 6) - message.size());
				std::ostringstream convert;
				FILE *out;
				out = popen(target.c_str(), "r");
				char buf[8000];
				while (fgets(buf, 100, out))
				{
					convert << buf;
				}
				pclose(out);
				Say("sy", convert.str());
			}
		}
	};
};

int main()
{
	ManBot client;
	client.Connect("82.239.138.44", 8300);
	client.Login("ManBot", "", "1337", "*", "liblobby V0.1");
	sleep(60);
	return 0;
};
