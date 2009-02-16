#include "TASServer.h"

const std::string myUser = "WhatisBot";
const std::string myPasswd = "WhatisBotPass";

struct TabReplacer
{
	void operator()(char& c) { if(c == '\t') c = ' '; }
};

class ManBot : public TASServer
{
	virtual void LoginSuccess(const std::string& username)
	{
		Join("sy");
	};
	
	virtual void LoginFail(const std::string& reason)
	{
		std::cout << "Reason: " << reason << std::endl;
		if (reason.find("Bad username/password") != std::string::npos)
		{
			RegisterAccount(myUser, myPasswd);
		}
	};
	
	virtual void AgreementEnd()
	{
		AgreementConfirm();
		Login(myUser, myPasswd, "1337", "*", "liblobby V0.1");
	};

	virtual void Said(const std::string& channame, const std::string& username, const std::string& message)
	{
		if (channame == "sy")
		{
			if (message.find("!whatis") == 0 && message.size() > 6)
			{
				const std::string target = "man -f "+message.substr(5, message.find(" \t\n", 6) - message.size());
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
					Say("sy", line);
				}
			}
			if (message.find("!quit") == 0)
			{
				exit(0);
			}
		}
	};
};

int main()
{
	ManBot client;
	client.Connect("taspringmaster.clan-sy.com", 8200);
	client.Login(myUser, myPasswd, "1337", "*", "liblobby V0.1");
	while (true)
	{
		sleep(1);
	}
	return 0;
};
