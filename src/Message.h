#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>

typedef std::vector<std::string> ArgVec;

class Message
{
public:
	Message(unsigned msgId=0);
	Message(const std::string& command, unsigned msgId=0);
	const std::string& GetCommand() const
	{
		return command;
	};
	unsigned GetId() const
	{
		return msgId;
	};

	std::string GetFullStr() const;
	
	void Push(const std::string&);

protected:
	std::string command;
	std::string args;
	unsigned msgId;
};

class InMessage : public Message
{
public:
	InMessage(const std::string& message);
	std::string GetWord() const;
	std::string GetSentence() const;
	
private:
	mutable size_t args_pos;
};

#endif
