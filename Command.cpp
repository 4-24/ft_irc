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
		else if (_password != "" && !user.is_authenticated()) // 인증되지 않은 사용자
			send_err(user, 300, "unauthenticated user. stop.");
		else if (!user.is_registered() && command != "NICK" && command != "USER") // 등록되지 않은 사용자
			send_err(user, 300, "register first");
		else if (command == "NICK")
			cmd_nick(user, params[0]);
		else if (command == "USER")
			cmd_user(user, params[0]);
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
		else if (command == "NAMES")
			cmd_names(user, params);
		else if (command == "PRIVMSG")
			cmd_privmsg(user, params);
		else if (command == "NOTICE")
			cmd_notice(user, params);
		else
			send_err(user, 300, "command not found");
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what();
	}
}

void	Server::cmd_pass(User &user, std::vector<std::string> params)
{
	if (params.size() == 1)
	{
		if (params[0] == _password)
		{
			user.set_authenticated(true);
			send_msg(user, 300, "Authenticated...");
		}
		else
			send_err(user, 300, "password incorrect");
	}
}

bool check_nick(std::string const &str) {
 	return (str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") ==
		std::string::npos) && (str.size() > 0 && str.size() < 9);
}

void	Server::cmd_nick(User &user, std::string param)
{
	if (!check_nick(param)) // 잘못된 닉네임
		send_err(user, 300, "invalid nick");
	else if (find_nickname(param) != -1) // 이미 존재하는 닉네임
		send_err(user, 300, "nickname already exists");
	else // 정상적인 닉네임
	{
		user.set_nickname(param);
		send_msg(user, 300, "nickname set");
		if (user.get_nickname().size() > 0 && user.get_username().size() > 0)
		{
			user.set_registered(true);
			send_user_info(user, "Welcome to the Internet Relay Network: ");
		}
	}
}

void	Server::cmd_user(User &user, std::string param)
{
	if (user.is_registered()) // 이미 등록된 유저
		send_err(user, 300, "already registered");
	else // 정상적인 유저
	{
		if (find_username(param) == -1)
		{
			user.set_username(param);
			send_msg(user, 300, "username set");
		}
		else // 기존에 등록된 유저로 로그인
		{
			replace_user(_users[find_username(param)], user);
		}
		if (user.get_nickname().size() > 0 && user.get_username().size() > 0)
		{
			user.set_registered(true);
			send_user_info(user, "Welcome to the Internet Relay Network: ");
		}
	}
}

void	Server::cmd_oper(User &user, std::vector<std::string> params)
{
	int user_idx = find_user_idx(user.get_fd());

	if (params.size() != 2)
		send_err(user, 300, "usage : ./oper [nick] [password]");
	if (params[0] != SUPER_NICK)
		send_err(user, 300, "wrong host nick name");
	if (params[1] != SUPER_PASS)
		send_err(user, 300, "wrong host password");
	_users[user_idx].set_admin(true);
	send_msg(user, 300, "Operator privileges have been obtained");
	std::cout << "\nUSER[" << user_idx << "] Operator privileges have been obtained\n" << std::endl;
}

void	Server::cmd_mode(User &user, std::vector<std::string> params)
{
	int user_idx = find_user_idx(user.get_fd());

	if (!_users[user_idx].is_admin())
		send_err(user, 300, "you're not operator");
	if (params[0].empty() || params[1].empty())
		send_err(user, 300, "usage : ./mode [option] [nick]");
	if ((params[0][0] == '+' || params[0][0] == '-' ) && params[0][1] == 'o' && params[0].size() == 2)
	{
		if (find_nickname(params[1]) == -1)
			send_err(user, 300, "that user does not exist");
		if (params[0][0] == '+')
			_users[find_nickname(params[1])].set_admin(true);
		else
			_users[find_nickname(params[1])].set_admin(false);
	}
	else
		send_err(user, 300, "usage : ./mode [option] [nick]");
}

void	Server::cmd_join(User &user, std::string param)
{
	if (param.size() > 0)
	{
		if (param[0] == '#')
		{
			if (user.get_room_idx() != -1) // 유저가 이미 방에 들어가있을 때
				send_err(user, 300, "already in a room");
			else
			{
				int i = find_room_idx(param);
				if (i == -1) // 방이 없을 때
				{
					Room	room(param);
					user.set_room_idx(_rooms.size());
					room.add_user(user);
					_rooms.push_back(room);
				}
				else // 방이 있을 때
				{
					user.set_room_idx(i);
					_rooms[i].add_user(user);
				}
				_rooms[find_room_idx(param)].get_user_list();
			}
		}
		else
			send_err(user, 300, "invalid room");
	}
}

void	Server::cmd_kick(User &user, std::vector<std::string> params)
{
	(void)user, (void)params; //TODO: Implement this
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
			send_err(user, 300, "not in a room");
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
				send_err(user, 300, "not in a room");
		}
		else
			send_err(user, 300, "invalid room");
	}
}

void	Server::cmd_names(User &user, std::vector<std::string> params)
{
	if (params.size() == 0)
	{
		if (!_rooms.empty())
		{
			for (unsigned long i = 0; i < _rooms.size(); i++)
			{
				if (_rooms[i].get_name() == "")
					continue;
				send_msg(user, 300, _rooms[i].get_name());
				send_msg(user, 300, "Users in the room : " + _rooms[user.get_room_idx()].get_user_list());
			}
		}
		else
			send_err(user, 300, "not in a room");
	}
	else
	{
		if (params[0][0] == '#')
		{
			send_msg(user, 300, "Room: " + params[0]);
			if (find_room_idx(params[0]) != -1)
			{
				send_msg(user, 300, "Users in the room : " + _rooms[user.get_room_idx()].get_user_list());
				_rooms[user.get_room_idx()].get_user_list();
			}
			else
				send_err(user, 300, "invalid room");
		}
		else
			send_err(user, 300, "invalid room");
	}
}

void	Server::cmd_privmsg(User &user, std::vector<std::string> params)
{
	if (params.size() < 2)
		send_err(user, 300, "Need more parameters");

	if (params[0][0] == '#') // 방에서 메시지를 보낼 때
	{
		if (find_room_idx(params[0]) == -1)
			send_err(user, 300, "No such room");
		else
			send_privmsg_to_room(user.get_fd(), find_room_idx(params[0]), params[1]);
	}
	else // 유저에게 메시지를 보낼 때
	{
		if (find_nickname(params[0]) == -1)
			send_err(user, 300, "No such user");
		else
			send_privmsg(_users[find_nickname(params[0])], params[1]);
	}
}

void	Server::cmd_notice(User &user, std::vector<std::string> params)
{
	if (params.size() < 2)
		send_err(user, 300, "Need more parameters");

	if (params[0][0] == '#') // 방에서 메시지를 보낼 때
	{
		if (find_room_idx(params[0]) == -1)
			return ;
		else
			send_notice_to_room(user.get_fd(), find_room_idx(params[0]), params[1]);
	}
	else // 유저에게 메시지를 보낼 때
	{
		if (find_nickname(params[0]) == -1)
			return ;
		else
			send_notice(_users[find_nickname(params[0])], params[1]);
	}
}

void	Server::quit(User &user)
{
	send_msg(user, 300, "Goodbye!");
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
	send_err(user, 300, "flood detected, please wait " + ss.str() + " seconds");
	return true;
}

void	Server::replace_user(User &old_user, User &new_user)
{
	int new_idx = find_user_idx(new_user.get_fd());
	int old_fd = old_user.get_fd();

	old_user.set_fd(new_user.get_fd());
	_users.erase(_users.begin() + new_idx); // 새로운 유저 삭제
	send_msg(old_user, 300, "Goodbye!");
	close(old_fd);
	_fds.erase(_fds.begin() + find_user_idx(old_fd));
	std::cout << "User " << old_fd << " disconnected." << std::endl;
	send_msg(new_user, 300, "user login: " + new_user.get_nickname());
}

void	Server::cmd_ping(User &user, const Message &msg)
{
	std::string name = SERV;
	if (msg.get_params().size() == 0)
		send_err(user, ERR_NOORIGIN, " :No origin specified");
	else
		send_pong(user, msg.get_params()[0]);
}

void	Server::cmd_pong(User &user, const Message &msg)
{
	if (msg.get_params().size() <= 0 || msg.get_params()[0] != SERV)
		return (send_err(user, ERR_NOSUCHSERVER, msg.get_params().size() > 0 ? msg.get_params()[0] : ""));
}
