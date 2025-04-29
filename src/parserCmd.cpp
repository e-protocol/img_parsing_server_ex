#include "include/parserCmd.h"
#include "include/imgProcess.hpp"

	/**
	 * @brief Construct a new Parser Cmd object, launched in separate thread on creation
	 * @param ptr pointer to Server object
	 */
  ParserCmd::ParserCmd(Server* server) : server_(server)
  {
    PrintVersion();
    thread_ = std::thread(&ParserCmd::parseInput, this);
  }

	/**
	 * @brief class destructor, wait for thread_ finished
	 */
	ParserCmd::~ParserCmd() { thread_.join(); }

/**
 * @brief func to parse input command string
 * 
 */
void ParserCmd::parseInput()
{
	while(true)
	{
		PrintTerminal("->", false);
		std::vector<std::string> args;
		ReadInput(args);

		switch(GetCmd(args))
		{
			case(0): PrintHelp(); break;
			case(1): return;
			case(2): std::system("clear"); break;
			case(3): PrintVersion(); break;
			case(4): server_->SetPort(args); break;
			case(5): PrintPort(); break;
			case(6): server_->Start(); break;
			case(7): server_->Stop(); break;
			default: PrintTerminal("Invalid command!\n"); break;
		}
	}
}

/**
 * @brief func to split input string into arguments
 * @param args splitted command arguments
 */
void ParserCmd::ReadInput(std::vector<std::string>& args)
{
  std::string input;
	getline(std::cin, input);
  std::stringstream ss(input);
  while(ss >> input) args.emplace_back(input);
}

/**
	 * @brief Get the index of command from cmd_ array
	 * 
	 * @param args - splitted command arguments
	 * @return int index of cmd_[]
	 */
	int ParserCmd::GetCmd(std::vector<std::string>& args)
	{
		if(args.empty()) return -1;
		ConcatCmd(args);

		for(int i = 0; i < CMD_SIZE; ++i)
		{
			if(args[0] == std::string(cmd_[i]))
				return i;
		}
		return -1;
	}

  /**
	 * @brief function to concat some arguments to match commands in cmd_[]
	 * @param args splitted command arguments
	 */
	void ParserCmd::ConcatCmd(std::vector<std::string>& args)
	{
		if(args.size() < 2)
      return;
		
		if(args[0] == "set" || args[0] == "get")
		{
			if(args[0] == "set" && args.size() < 3)
				return;

			args[0] = args[0] + " " + args[1];
			args.erase(args.begin() + 1);
		}
	}

  /**
	 * @brief func to print commands syntax and explanation in terminal
	 * 
	 */
	void ParserCmd::PrintHelp()
	{
		PrintTerminal("Command list:\n");

		auto print_func = [this](int i)->std::string
		{
			std::string str = std::string(cmd_[i]);

			switch(i)
			{
				case(0): str += " - view command list"; break;
				case(1): str += " - close app"; break;
				case(2): str += " - clear terminal"; break;
				case(3): str += " - view app version"; break;
				case(4): str += " - set listening port, ex.: set port 8080"; break;
				case(5): str += " - get listeninig port, ex.: get port"; break;
				case(6): str += " - starts server"; break;
				case(7): str += " - stops server"; break;
				default: break;
			}
			return str;
		};

		for(int i = 0; i < CMD_SIZE; ++i)
			PrintTerminal(print_func(i));
	}

  /**
   * @brief method to print app version
   * 
   */

	void ParserCmd::PrintVersion()
	{
		PrintTerminal("opencv image parsing server");
		PrintTerminal("version 1.0");
		PrintTerminal("Using Boost v" + std::to_string(BOOST_VERSION));
  }


  	/**
	 * @brief prints current listening port
	 * 
	 */
	void ParserCmd::PrintPort() 
  {
		 PrintTerminal("listen port: " + std::to_string(server_->GetPort()));
  }