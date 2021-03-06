#include "TASServer.h"

#include <unistd.h>

const std::string myUser = "FortuneBot";
const std::string myPasswd = "FortuneBot";

struct TabReplacer
{
	void operator()(char& c) { if(c == '\t') c = ' '; }
};

class FortuneBot : public TASServer
{
	virtual void LoginSuccess(const std::string& username)
	{
		Join("bottest");
	};
	
	virtual void LoginFail(const std::string& reason)
	{
		std::cout << "Reason: " << reason << std::endl;
		if (reason.find("Bad username/password") != std::string::npos)
		{
			RegisterAccount(myUser, myPasswd);
			Login(myUser, myPasswd, "1337", "*", "liblobby V0.1");
		}
	};
	
	virtual void AgreementEnd()
	{
		AgreementConfirm();
		Login(myUser, myPasswd, "1337", "*", "liblobby V0.1");
	};

	virtual void Said(const std::string& channame, const std::string& username, const std::string& message)
	{
		if (message.find("!fortune") == 0)
		{
			const std::string target = "fortune";
			std::ostringstream convert;
			FILE *out;
			out = popen(target.c_str(), "r");
			char buf[8000];
			while (fgets(buf, 100, out))
			{
				convert << buf;
			}
			pclose(out);
			std::istringstream stringbuf(convert.str());
			
			while (!stringbuf.eof())
			{
				std::string line;
				std::getline(stringbuf,line);
				SayEx(channame, line);
			}
		}
		else if (message.find("!cookie") == 0)
		{
			SayEx(channame, " gives a cookie to "+username);
		}
		else if (message.find("!inet") == 0)
		{
			std::istringstream stringbuf(message);
			std::string buf;
			stringbuf >> buf;
			stringbuf >> buf;
			SayEx(channame, ": "+buf+" gains 1 free internets");
		}
		else if (username == "Auswaschbar")
		{
			// admin commands
			if (message.find("!quit") == 0)
			{
				exit(0);
			}
			else if (message.find("!restart") == 0)
			{
				char* args[1];
				args[0] = "fortunebot";
				char* env[0];
				execve ("fortunebot", args, env);
				SaidPrivate("Auswaschbar", "Fehler");
				exit(0);
			}
			else if (message.find("!addchannel") == 0)
			{
				std::istringstream stringbuf(message);
				std::string buf;
				stringbuf >> buf;
				stringbuf >> buf;
				Join(buf);
			}
		}
	};
};

int main()
{
	FortuneBot client;
	client.Connect("taspringmaster.clan-sy.com", 8200);
	client.Login(myUser, myPasswd, "1337", "*", "liblobby V0.1");
	while (true)
	{
		client.Ping();
		sleep(10);
	}
	return 0;
};
