#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>

typedef std::vector<std::string> ArgVec;

class Message
{
	public:
		Message(const std::string& message);
		Message(const std::string& command, const ArgVec& arguments, unsigned msgId=0);
		const std::string& GetCommand() const
		{
			return command;
		};
		unsigned GetId() const
		{
			return msgId;
		};

		std::string GetWord() const;
		std::string GetSentence() const;

		std::string GetFullStr() const;
	private:
		std::string command;
		mutable size_t args_pos;
		std::string args;
		unsigned msgId;
};

#endif
