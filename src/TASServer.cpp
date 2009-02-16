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

void TASServer::RenameAccount(const std::string& newname)
{
	Message msg("RENAMEACCOUNT");
	msg.Push(newname);
	SendMessage(msg);
}

void TASServer::ChangePasword(const std::string& oldpass, const std::string& newpass)
{
	Message msg("CHANGEPASSWORD");
	boost::md5 checksumold(oldpass.c_str());
	msg.Push(Base64::encode((const char*)checksumold.digest().value()));
	boost::md5 checksumnew(newpass.c_str());
	msg.Push(Base64::encode((const char*)checksumnew.digest().value()));
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

void TASServer::SayPrivate(const std::string& username, const std::string& message)
{
	Message msg("SAYPRIVATE");
	msg.Push(username);
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
		const std::string server = msg.GetWord();
		const std::string spring = msg.GetWord();
		int port, mode;
		StringConvert(msg.GetWord(), port);
		StringConvert(msg.GetWord(), mode);
		TASServerMsg(server, spring, port, mode);
	}
	else if (msg.GetCommand() == "SERVERMSG")
	{
		ServerMessage(msg.GetSentence());
	}
	else if (msg.GetCommand() == "SERVERMSGBOX")
	{
		const std::string message = msg.GetSentence();
		ServerMessageBox(message, msg.GetSentence());
	}
	else if (msg.GetCommand() == "ACCEPTED")
	{
		LoginSuccess(msg.GetWord());
	}
	else if (msg.GetCommand() == "DENIED")
	{
		LoginFail(msg.GetSentence());
	}
	else if (msg.GetCommand() == "LOGININFOEND")
	{
		LoginInfoEnd();
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
	else if (msg.GetCommand() == "SAYPRIVATE")
	{
		const std::string user = msg.GetWord();
		MeSaidPrivate(user, msg.GetSentence());
	}
	else if (msg.GetCommand() == "SAIDPRIVATE")
	{
		const std::string user = msg.GetWord();
		SaidPrivate(user, msg.GetSentence());
	}
	else if (msg.GetCommand() == "ADDUSER")
	{
		const std::string user = msg.GetWord();
		const std::string country = msg.GetWord();
		AddUser(user, country, msg.GetSentence());
	}
	
	else if (msg.GetCommand() == "REMOVEUSER")
	{
		RemoveUser(msg.GetSentence());
	}
	else if (msg.GetCommand() == "MOTD")
	{
		Motd(msg.GetSentence());
	}
}
