# include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc == 2) // argc == 3일때 에러 핸들링 (TODO// ./ircserv port password)
	{
		std::stringstream ss;
		ss << argv[1];
		int num;
		ss >> num;

		Server server(num);
		server.start();
		return 0;
	}
	else
		std::cout << "[ERROR] ircserv: bad arguments" << std::endl;
	return -1;
}
