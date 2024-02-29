#pragma once
#include <iostream>
#include <vector>


enum class LogSeverity
{
	Info,
	Warning,
	Error,
	Unknown
};

class LogMessage
{
public:
	LogSeverity severity;
	LogMessage(LogSeverity sev) : severity(sev) {}
	virtual std::string GetStr() = 0;
};

class Logger
{
public:
	virtual void Log(LogMessage* msg) = 0;
	virtual std::vector<LogMessage*> GetAllMessages() = 0;
};

class BasicLogger : public Logger
{
	std::vector<LogMessage*> messages;
public:
	void Log(LogMessage* message)
	{
		messages.push_back(message);
	}
	std::vector<LogMessage*> GetAllMessages() { return messages; };
};