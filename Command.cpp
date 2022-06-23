# include "Server.hpp"

void	Server::execute(User &user, Message message)
{
	std::string					prefix = message.get_prefix();
	std::string					command = message.get_command();
	std::vector<std::string>	params = message.get_params();

	try
	{
		if (command == "QUIT")
			quit(user);
		else if (command == "PING")
			cmd_ping(user, user.get_message());
		else if (command == "PONG")
			cmd_pong(user, user.get_message());
		else if (command == "PASS")
			cmd_pass(user, params);
		else if (command == "NICK")
			cmd_nick(user, params[0]);
		else if (command == "USER")
			cmd_user(user, params);
		else if (!user.is_authenticated() && !user.is_registered() && command != "NICK" && command != "USER") // 등록되지 않은 사용자
			send_err(user, ERR_NOTREGISTERED, "You have not registered. register first.");
		else if (is_flooding(user)) // 플러딩 체크
			return ;
		else if (command == "OPER")
			cmd_oper(user, params);
		else if (command == "MODE")
			cmd_mode(user, params);
		else if (command == "JOIN")
			cmd_join(user, params[0]);
		else if (command == "KICK")
			cmd_kick(user, params);
		else if (command == "PART")
			cmd_part(user, params);
		else if (command == "PRIVMSG")
			cmd_privmsg(user, params);
		else if (command == "NOTICE")
			cmd_notice(user, params);
		else
			send_err(user, ERR_UNKNOWNCOMMAND, "command not found");
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what();
	}
}

void	Server::cmd_pass(User &user, std::vector<std::string> params)
{
	if (user.is_authenticated())
		send_err(user, ERR_ALREADYREGISTRED, "You may not reregister");
	if (params.size() == 1)
	{
		if (params[0] == _password)
		{
			user.set_authenticated(true);
			send_msg(user, RPL_NONE, "Authenticated...");
		}
		else
			send_err(user, ERR_PASSWDMISMATCH, "password incorrect");
	}
}

bool check_nick(std::string const &str) {
 	return (str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") ==
		std::string::npos) && (str.size() > 0 && str.size() < 9);
}

void	Server::cmd_nick(User &user, std::string param)
{
	if (!check_nick(param)) // 잘못된 닉네임
		send_err(user, ERR_ERRONEUSNICKNAME, "invalid nick");
	else if (find_nickname(param) != -1) // 이미 존재하는 닉네임
		send_err(user, ERR_NICKNAMEINUSE, "nickname already exists");
	else // 정상적인 닉네임
	{
		user.set_nickname(param);
		if (user.get_nickname().size() > 0 && user.get_username().size() > 0)
		{
			user.set_registered(true);
			send_motd(user);
		}
	}
}

void	Server::cmd_user(User &user, std::vector<std::string> params)
{
	if (user.is_registered()) // 이미 등록된 유저
		send_err(user, ERR_ALREADYREGISTRED, "You may not reregister");
	if (params.size() != 4)
		send_err(user, ERR_NEEDMOREPARAMS, "Not enough parameters");
	else // 정상적인 유저
	{
		if (find_username(params[0]) == -1)
		{
			user.set_username(params[0]);
			user.set_realname(params[3]);
		}
		else
		{
			send_err(user, ERR_ALREADYREGISTRED, "You may not reregister");
		}
		if (user.get_nickname().size() > 0 && user.get_username().size() > 0)
		{
			user.set_registered(true);
			send_motd(user);
		}
	}
}

void	Server::cmd_oper(User &user, std::vector<std::string> params)
{
	int user_idx = find_user_idx(user.get_fd());

	if (params.size() != 2)
		send_err(user, ERR_NEEDMOREPARAMS, "usage : ./oper [nick] [password]");
	if (params[0] != SUPER_NICK)
		send_err(user, ERR_WRONGUSERNAME, "wrong host nick name");
	if (params[1] != SUPER_PASS)
		send_err(user, ERR_PASSWDMISMATCH, "wrong host password");
	_users[user_idx].set_admin(true);
	send_msg(user, RPL_YOUREOPER, "Operator privileges have been obtained");
	std::cout << "\nUSER[" << user_idx << "] Operator privileges have been obtained\n" << std::endl;
}

void	Server::cmd_mode(User &user, std::vector<std::string> params)
{
	int user_idx = find_user_idx(user.get_fd());

	if (!_users[user_idx].is_admin())
		send_err(user, ERR_NOPRIVILEGES, "you're not operator");
	if (params[0].empty() || params[1].empty())
		send_err(user, ERR_NEEDMOREPARAMS, "usage : ./mode [option] [nick]");
	if ((params[0][0] == '+' || params[0][0] == '-' ) && params[0][1] == 'o' && params[0].size() == 2)
	{
		if (find_nickname(params[1]) == -1)
			send_err(user, ERR_NOSUCHNICK, "that user does not exist");
		if (params[0][0] == '+')
			_users[find_nickname(params[1])].set_admin(true);
		else
			_users[find_nickname(params[1])].set_admin(false);
	}
	else
		send_err(user, ERR_NEEDMOREPARAMS, "usage : ./mode [option] [nick]");
}

void	Server::cmd_join(User &user, std::string param)
{
	if (param.size() > 0)
	{
		if (param[0] == '#')
		{
			if (user.get_room_idx() != -1) // 유저가 이미 방에 들어가있을 때
				return ;
			else
			{
				int i = find_room_idx(param);
				if (i == -1) // 방이 없을 때
				{
					Room	room(param);
					user.set_room_idx(_rooms.size());
					room.add_user(user);
					_rooms.push_back(room);
					room.send_all(":" + user.get_nickname() + " JOIN " + room.get_name() + "\n");
					send_msg(user, RPL_NOTOPIC, param + " :No topic is set");
				}
				else // 방이 있을 때
				{
					if (_rooms[i].get_users().size() > 10)
						send_err(user, ERR_CHANNELISFULL, param + " :You have joined too many channels");
					user.set_room_idx(i);
					_rooms[i].add_user(user);
					_rooms[i].send_all(":" + user.get_nickname() + " JOIN " + _rooms[i].get_name() + "\n");
					send_msg(user, RPL_NOTOPIC, param + " :No topic is set");
				}
				_rooms[find_room_idx(param)].get_user_list();
			}
		}
		else
			send_err(user, ERR_NOSUCHCHANNEL, param + " :No such channel");
	}
}

void	Server::cmd_kick(User &user, std::vector<std::string> params)
{
	int	room_idx = find_room_idx(params[0]);

	if (params.size() < 2)
		send_err(user, ERR_NEEDMOREPARAMS, "KICK :Not enough parameter");
	else if (room_idx == -1)
		send_err(user, 403, params[0] + " :No such channel");
	else if (!user.is_admin())
		send_err(user, 482, params[0] + " :You're not channel operator");
	else if (room_idx != user.get_room_idx())
		send_err(user, 482, params[0] + " :You're not on that channel");
	else if (_rooms[room_idx].is_user(params[1]) == false)
		send_err(user, 441, params[1] + " :No such nickname");
	else {
		_rooms[room_idx].remove_user(params[1]);
		send_msg(user, RPL_NONE, "KICK :" + params[1]);
	}
}

void	Server::cmd_part(User &user, std::vector<std::string> params)
{
	if (params.size() == 0)
	{
		if (user.get_room_idx() != -1)
		{
			_rooms[user.get_room_idx()].remove_user(user);
			user.set_room_idx(-1);
		}
		else
			send_err(user, ERR_NOTONCHANNEL, "You're not on that channel");
	}
	else
	{
		if (params[0][0] == '#')
		{
			if (user.get_room_idx() != -1)
			{
				_rooms[user.get_room_idx()].remove_user(user);
				user.set_room_idx(-1);
			}
			else
				send_err(user, ERR_NOTONCHANNEL, "You're not on that channel");
		}
		else
			send_err(user, ERR_NOSUCHCHANNEL, "No such channel");
	}
}

void	Server::cmd_privmsg(User &user, std::vector<std::string> params)
{
	if (!params.size() || params[0][0] == ':')
		send_err(user, ERR_NORECIPIENT,  " :No recipient given (" + params[0] + ")");
	if (params.size() < 2)
		send_err(user, ERR_NOTEXTTOSEND, params[0] + " " + params[1] + " :No text to send");

	if  (params[0][0] == '#') // 방에서 메시지를 보낼 때
	{
		if (find_room_idx(params[0]) == -1)
			send_err(user, ERR_CANNOTSENDTOCHAN, "PRIVMSG " + params[0] + " :Cannot send to channel");
		else
			send_privmsg_to_room(user, find_room_idx(params[0]), params[1]);
	}
	else // 유저에게 메시지를 보낼 때
	{
		if (find_nickname(params[0]) == -1)
			send_err(user, ERR_NOSUCHNICK, "PRIVMSG " + params[0] + " :No such nickname");
		else
			send_privmsg(_users[find_nickname(params[0])], user, params[1]);
	}
}

void	Server::cmd_notice(User &user, std::vector<std::string> params)
{
	if (!params.size() || params[0][0] == ':')
		send_err(user, ERR_NORECIPIENT,  " :No recipient given (" + params[0] + ")");
	if (params.size() < 2)
		send_err(user, ERR_NOTEXTTOSEND, params[0] + " " + params[1] + " :No text to send");

	if (find_nickname(params[0]) == -1)
		send_err(user, ERR_NOSUCHNICK, "NOTICE " + params[0] + " :No such nickname");
	else
		send_notice(_users[find_nickname(params[0])], user, params[1]);
}

void	Server::quit(User &user)
{
	if (user.get_room_idx() != -1)
		cmd_part(user, std::vector<std::string>(0));
	send_msg(user, RPL_NONE, "Goodbye!");
	close(user.get_fd());
	std::cout << "User " << user.get_fd() << " disconnected." << std::endl;

	int user_idx = find_user_idx(user.get_fd());
	int fd_idx = find_fd_idx(user.get_fd());
	_users.erase(_users.begin() + user_idx);
	_fds.erase(_fds.begin() + fd_idx);
}

bool	Server::is_flooding(User &user)
{
	time_t t = time(0);

	if (t - user.get_last_message_time() > user.get_message_timeout())
	{
		user.set_last_message_time(t);
		user.set_message_timeout(1);
		return false;
	}
	user.set_last_message_time(t);
	if (user.get_message_timeout() < (2 << 6))
		user.set_message_timeout(user.get_message_timeout() * 2);
	std::stringstream ss;
	ss << user.get_message_timeout();
	send_err(user, ERR_FLOOD, "flood detected, please wait " + ss.str() + " seconds");
	return true;
}

void	Server::cmd_ping(User &user, const Message &msg)
{
	std::string name = SERV;
	if (msg.get_params().size() == 0)
		send_err(user, ERR_NOORIGIN, "No origin specified");
	else
		send_pong(user, msg.get_params()[0]);
}

void	Server::cmd_pong(User &user, const Message &msg)
{
	if (msg.get_params().size() <= 0 || msg.get_params()[0] != SERV)
		return (send_err(user, ERR_NOSUCHSERVER, msg.get_params().size() > 0 ? msg.get_params()[0] : ""));
}
