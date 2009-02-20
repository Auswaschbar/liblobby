#include "Message.h"


#include <assert.h>
#include <sstream>


Message::Message(unsigned newid) : msgId(newid)
{
}

Message::Message(const std::string& _command, unsigned newid) : command(_command), msgId(newid)
{
}

std::string Message::GetFullStr() const
{
	std::ostringstream out;
	if (msgId > 0)
	{
		out << '#' << msgId << ' ';
	}
	out << command << ' ' << args << std::endl;
	return out.str();
}

void Message::Push(const std::string& word)
{
	if (!args.empty())
		args+= " ";
	args += word;
}


InMessage::InMessage(const std::string& message) : args_pos(0)
{
	args_pos = 0;
	std::istringstream inbuf(message);
	if (message.at(0) == '#')
	{
		inbuf.seekg(1, std::ios::beg);
		inbuf >> msgId;
	}
	inbuf >> command;

	size_t args_start = (size_t)inbuf.tellg()+1;
	if (message.size() > args_start)
		args = message.substr(args_start);
}


std::string InMessage::GetWord() const
{
	size_t prev = args_pos;
	size_t next = std::min(args.find(" ", args_pos+1), args.size());
	args_pos = next+1;
	//assert(args_pos != std::string::npos);
	return args.substr(prev, next-prev);
}

std::string InMessage::GetSentence() const
{
	size_t prev = args_pos;
	size_t next = std::min(args.find_first_of("\t\n", args_pos+1), args.size());
	args_pos = next+1;
	//assert(args_pos != std::string::npos);
	return args.substr(prev, next-prev);
}
