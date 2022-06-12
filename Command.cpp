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
			cmd_nick(user, params[0]);
		else if (command == "USER")
			cmd_user(user, params[0]);
		else if (command == "OPER")
			cmd_oper(fd, params);
		else if (command == "MODE")
			cmd_mode(fd, params);
		else if (command == "JOIN")
			cmd_join(user, params[0]);
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
	send(fd, (GREEN + message + RESET + "\n").c_str(), message.size() + 11, 0);
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

bool check_nick(std::string const &str) {
 	return (str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") ==
		std::string::npos) && (str.size() > 0 && str.size() < 9);
}

void	Server::cmd_nick(User &user, std::string param)
{
	if (!check_nick(param))
		send_err(user.get_fd(), "invalid nick");
	else
	{
		user.set_nickname(param);
		send_msg(user.get_fd(), "nickname set");

		if (user.get_nickname().size() > 0 && user.get_username().size() > 0)
		{
			user.set_registered(true);
			send_msg(user.get_fd(), "registered");
		}
	}
}

void	Server::cmd_user(User &user, std::string param)
{
	if (user.is_registered())
		send_err(user.get_fd(), "already registered");
	else
	{
		user.set_username(param);
		send_msg(user.get_fd(), "username set");

		if (user.get_nickname().size() > 0 && user.get_username().size() > 0)
		{
			user.set_registered(true);
			send_msg(user.get_fd(), "registered");
		}
	}
}

void	Server::cmd_oper(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_mode(int fd, std::vector<std::string> params)
{
	(void)fd, (void)params; //TODO: Implement this
}

void	Server::cmd_join(User &user, std::string param)
{
	if (param.size() > 0)
	{
		if (param[0] == '#')
		{
			if (user.get_room_idx() != -1) // 유저가 이미 방에 들어가있을 때
				send_err(user.get_fd(), "already in a room");
			else
			{
				int i = find_room_idx(param);
				if (i == -1) // 방이 없을 때
				{
					Room	room(param);
					user.set_room_idx(i);
					room.add_user(user);
					_rooms.push_back(room);
				}
				else // 방이 있을 때
				{
					user.set_room_idx(i);
					_rooms[i].add_user(user);
				}
				_rooms[find_room_idx(param)].show_info();
			}
		}
		else
			send_err(user.get_fd(), "invalid room");
	}
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

	int user_idx = find_user_idx(fd);
	int fd_idx = find_fd_idx(fd);
	_users.erase(_users.begin() + user_idx);
	std::cout << "Users left: " << _users.size() << std::endl;
	_fds.erase(_fds.begin() + fd_idx);
	std::cout << "Fds left: " << _fds.size() << std::endl;
}
