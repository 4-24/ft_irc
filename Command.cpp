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

void	Server::cmd_nick(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_user(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_oper(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_mode(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_join(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_kick(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_part(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_names(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_privmsg(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_notice(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::quit(int fd)
{
	send_msg(fd, "Goodbye!");
	close(fd);
	std::cout << "User " << fd << " disconnected." << std::endl;

	std::cout << "Users current: " << _users.size() << std::endl;
	std::cout << "Fds current: " << _fds.size() << std::endl;

	int idx = find_user_idx(fd);
	int fd_idx = find_fd_idx(fd);
	delete *(_users.begin() + idx);
	std::cout << "deleted user: " << fd << std::endl;
	_users.erase(_users.begin() + idx);
	std::cout << "Users left: " << _users.size() << std::endl;
	_fds.erase(_fds.begin() + fd_idx);
	std::cout << "Fds left: " << _fds.size() << std::endl;
}
