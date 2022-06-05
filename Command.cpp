# include "Server.hpp"

void	Server::execute(User &user, Message message)
{
	std::string					prefix = message.get_prefix();
	std::string					command = message.get_command();
	std::vector<std::string>	params = message.get_params();
	int							fd = user.get_fd();

	try
	{
		if (command == "PASS")
			cmd_pass(fd, params);
		else if (!user.is_authenticated())
			throw std::runtime_error("unauthenticated user. stop.\n");
		if (!user.is_registered() && command != "NICK" && command != "USER")
			throw std::runtime_error("register first\n");
		else if (command == "NICK")
			cmd_nick(fd, params);
		else if (command == "USER")
			cmd_user(fd, params);
		else if (command == "OPER")
			cmd_oper(fd, params);
		else if (command == "MODE")
			cmd_mode(fd, params);
		else if (command == "JOIN")
			cmd_join(fd, params);
		else if (command == "KICK")
			cmd_kick(fd, params);
		else if (command == "PART")
			cmd_part(fd, params);
		else if (command == "NAMES")
			cmd_names(fd, params);
		else if (command == "PRIVMSG" || command == "NOTICE")
			cmd_privmsg(fd, params);
		else if (command == "QUIT")
			cmd_quit(fd);
		else
			throw std::runtime_error("command not found\n");
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what();
	}
}

void	Server::cmd_pass(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_nick(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_user(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_oper(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_mode(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_join(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_kick(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_part(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_names(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_privmsg(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_notice(int user_fd, std::vector<std::string> params)
{
	(void)user_fd, (void)params; //TODO: Implement this
}

void	Server::cmd_quit(int user_fd)
{
	(void)user_fd; //TODO: Implement this
}
