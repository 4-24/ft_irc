# include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		std::stringstream ss;
		ss.str("");
		ss << argv[1];
		int num;
		ss >> num;

		Server server(num);
		server.start();
		return 0;
	}
	else if (argc == 3)
	{
		std::stringstream ss;
		ss.str("");
		ss << argv[1];
		int num;
		ss >> num;
		std::string password = argv[2];

		Server server(num, password);
		server.start();
		return 0;
	}
	else
		std::cout << "[ERROR] ircserv: bad arguments" << std::endl;
	return -1;
}
