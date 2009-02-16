#include "TASServer.h"

#include <sstream>

#include "md5.hpp"
#include "base64.h"

template<typename T>
void StringConvert(const std::string& input, T& t)
{
	std::istringstream stream(input);
	stream >> t;
}

TASServer::TASServer()
{
};

void TASServer::RegisterAccount(const std::string& user, const std::string& passwd)
{
	Message msg("REGISTER");
	msg.Push(user);
	boost::md5 checksum(passwd.c_str());
	msg.Push(Base64::encode((const char*)checksum.digest().value()));
	SendMessage(msg);
}

void TASServer::AgreementConfirm()
{
	Message msg("CONFIRMAGREEMENT");
	SendMessage(msg);
}

void TASServer::Login(const std::string& user, const std::string& passwd, const std::string& cpu, const std::string& localIP, const std::string& lobbyName)
{
	Message msg("LOGIN");
	msg.Push(user);
	boost::md5 checksum(passwd.c_str());
	msg.Push(Base64::encode((const char*)checksum.digest().value()));
	msg.Push(cpu);
	msg.Push(localIP);
	msg.Push(lobbyName);
	SendMessage(msg);
}

void TASServer::Join(const std::string& channel, const std::string& key)
{
	Message msg("JOIN");
	msg.Push(channel);
	SendMessage(msg);
}

void TASServer::Leave(const std::string& channel)
{
	Message msg("LEAVE");
	msg.Push(channel);
	SendMessage(msg);
}

void TASServer::Say(const std::string& channame, const std::string& message)
{
	Message msg("SAY");
}

void TASServer::SayEx(const std::string& channame, const std::string& message)
{
	Message msg("SAYEX");
	msg.Push(channame);
	msg.Push(message);
	SendMessage(msg);
}

void TASServer::Ping()
{
	Message msg("PING");
	SendMessage(msg);
}

void TASServer::MessageRecieved(const InMessage& msg)
{
	if (msg.GetCommand() == "TASServer")
	{
		/*serverVersion = msg.GetWord();
		springVersion = msg.GetWord();
		StringConvert(msg.GetWord(), UDPPort);
		StringConvert(msg.GetWord(), servermode);*/
	}
	else if (msg.GetCommand() == "ACCEPTED")
	{
		LoginSuccess(msg.GetWord());
	}
	else if (msg.GetCommand() == "DENIED")
	{
		LoginFail(msg.GetSentence());
	}
	else if (msg.GetCommand() == "JOIN")
	{
		JoinSuccess(msg.GetWord());
	}
	else if (msg.GetCommand() == "JOINFAILED")
	{
		JoinFail(msg.GetWord(), msg.GetSentence());
	}
	else if (msg.GetCommand() == "SAID")
	{
		const std::string channel = msg.GetWord();
		const std::string user = msg.GetWord();
		Said(channel, user, msg.GetSentence());
	}
	else if (msg.GetCommand() == "REGISTRATIONDENIED")
	{
		RegisterFail(msg.GetSentence());
	}
	else if (msg.GetCommand() == "REGISTRATIONACCEPTED")
	{
		RegisterSuccess();
	}
	else if (msg.GetCommand() == "AGREEMENT")
	{
		Agreement(msg.GetSentence());
	}
	else if (msg.GetCommand() == "AGREEMENTEND")
	{
		AgreementEnd();
	}
	else if (msg.GetCommand() == "PONG")
	{
		Pong();
	}
	else if (msg.GetCommand() == "SAIDEX")
	{
		const std::string channel = msg.GetWord();
		const std::string user = msg.GetWord();
		Said(channel, user, msg.GetSentence());
	}
	else if (msg.GetCommand() == "MOTD")
	{
	}
}
