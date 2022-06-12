# include "Server.hpp"

void	Server::execute(User &user, Message message)
{
	std::string					prefix = message.get_prefix();
	std::string					command = message.get_command();
	std::vector<std::string>	params = message.get_params();
	int							fd = user.get_fd();

	try
	{
		if (command == "QUIT")
			quit(fd);
		else if (command == "PASS")
			cmd_pass(user, params);
		else if (_password != "" && !user.is_authenticated())
			send_err(fd, "unauthenticated user. stop.");
		else if (!user.is_registered() && command != "NICK" && command != "USER")
			send_err(fd, "register first");
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
		else
			send_err(fd, "command not found");
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what();
	}
}

void	Server::send_msg(int fd, std::string message)
{
	send(fd, (GREEN + message + RESET + "\n").c_str(), message.size() + 13, 0);
}

void	Server::send_err(int fd, std::string error)
{
	send(fd, (YELLOW + error + RESET + "\n").c_str(), error.size() + 11, 0);
	throw std::runtime_error((error + "\n").c_str());
}

void	Server::cmd_pass(User &user, std::vector<std::string> params)
{
	if (params.size() == 1)
	{
		if (params[0] == _password)
		{
			user.set_authenticated(true);
			send_msg(user.get_fd(), "Authenticated...");
		}
		else
			send_err(user.get_fd(), "password incorrect");
	}
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

void	Server::quit(int user_fd)
{
	send_msg(user_fd, "Goodbye!");
	close(user_fd);
	std::cout << "User " << user_fd << " disconnected." << std::endl;

	std::cout << "Users current: " << _users.size() << std::endl;
	std::cout << "Fds current: " << _fds.size() << std::endl;

	int i = find_user_idx(user_fd);
	delete *(_users.begin() + i);
	std::cout << "deleted user: " << user_fd << std::endl;
	_users.erase(_users.begin() + i);
	std::cout << "Users left: " << _users.size() << std::endl;
	_fds.erase(_fds.begin() + i);
	std::cout << "Fds left: " << _fds.size() << std::endl;
}
