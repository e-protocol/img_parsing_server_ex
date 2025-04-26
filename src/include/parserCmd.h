#pragma once
#include <sstream>
#include <stdio.h>
#include <thread>
#include <vector>
#include "server.h"
#define CMD_SIZE 10

/**
 * @brief class for parsing terminal commands in separate thread
 * @details This class also executes commands, commands are stored as
 * const char* cmd_[]. Defined CMD_SIZE is the lenght of cmd_[].
 */
class ParserCmd
{
	Server* server_ = nullptr;
	std::thread thread_;
	const char* cmd_[CMD_SIZE] = { "help", "exit", "clear", "version", "set port", "get port", 
																"set host", "get host", "start", "stop" };
public:
	ParserCmd() = delete;
	~ParserCmd() { thread_.join(); }
	ParserCmd(Server* server);
void parseInput();

private:
	void ReadInput(std::vector<std::string>& args);
	int GetCmd(std::vector<std::string>& args);
	void ConcatCmd(std::vector<std::string>& args);
	void PrintHelp();
	void PrintVersion();
	void PrintPort();
};