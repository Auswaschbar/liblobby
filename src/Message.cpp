#include "Message.h"


#include <sstream>

Message::Message(const std::string& message) : msgId(0)
{
	args_pos = 0;
	std::istringstream inbuf(message);
	if (message.at(0) == '#')
	{
		inbuf.seekg(1, std::ios::beg);
		inbuf >> msgId;
	}
	std::string command;
	inbuf >> command;

	args = message.substr(inbuf.tellg());
}

Message::Message(const std::string& _command, const ArgVec& arguments, unsigned newid) : msgId(newid), command(_command)
{
	std::ostringstream out;
	for (ArgVec::const_iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		out << *it << ' ';
	}
	out << std::endl;
	args = out.str();
}

std::string Message::GetWord() const
{
	size_t prev = args_pos++;
	args_pos = args.find_first_of(args_pos, ' ');
	return args.substr(prev, args_pos);
}

std::string Message::GetSentence() const
{
	size_t prev = args_pos++;
	args_pos = args.find_first_of(args_pos, '\t');
	return args.substr(prev, args_pos);
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

