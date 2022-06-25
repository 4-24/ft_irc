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
		else if (!user.is_authenticated() || (!user.is_registered() && command != "NICK" && command != "USER")) // 등록되지 않은 사용자
			send_err(user, ERR_NOTREGISTERED);
		else if (command == "OPER")
			cmd_oper(user, params);
		else if (command == "MODE")
			cmd_mode(user, params);
		else if (is_flooding(user)) // 플러딩 체크
			return ;
		else if (command == "JOIN")
			cmd_join(user, params);
		else if (command == "KICK")
			cmd_kick(user, params);
		else if (command == "PART")
			cmd_part(user, params[0]);
		else if (command == "PRIVMSG")
			cmd_privmsg(user, params);
		else if (command == "NOTICE")
			cmd_notice(user, params);
		else if (command == "NAMES")
			cmd_names(user, params);
		else if (command == "LIST")
			cmd_list(user, params);
		else if (command == "TOPIC")
			cmd_topic(user, params);
		else
			send_err(user, ERR_UNKNOWNCOMMAND);
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what();
	}
}

void	Server::cmd_pass(User &user, std::vector<std::string> params) // o.k
{
	if (user.is_authenticated())
		send_err(user, ERR_ALREADYREGISTRED(user.get_nickname()));
	if (params.size() == 1)
	{
		if (params[0] == _password)
		{
			user.set_authenticated(true);
			send_msg(user, RPL_NONE((std::string)"Authenticated..."));
		}
		else
			send_err(user, ERR_PASSWDMISMATCH(user.get_nickname()));
	}
}

bool check_nick(std::string const &str) {
 	return (str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") ==
		std::string::npos) && (str.size() > 0 && str.size() < 9);
}

void	Server::cmd_nick(User &user, std::string param) // o.k
{
	if (param == "")
		send_err(user, ERR_NONICKNAMEGIVEN);
	if (!check_nick(param)) // 잘못된 닉네임
		send_err(user, ERR_ERRONEUSNICKNAME(param));
	else if (find_nickname(param) != -1) // 이미 존재하는 닉네임
		send_err(user, ERR_NICKNAMEINUSE(param));
	else if (find_nickname(param) != -1 && _users[find_nickname(param)].is_registered())
		send_err(user, ERR_NICKCOLLISION(param));
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

void	Server::cmd_user(User &user, std::vector<std::string> params) // o.k
{
	if (params.size() != 4)
		send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "USER"));
	if (user.is_registered()) // 이미 등록된 유저
		send_err(user, ERR_ALREADYREGISTRED(user.get_nickname()));
	else // 정상적인 유저
	{
		if (find_username(params[0]) == -1)
		{
			user.set_username(params[0]);
			user.set_realname(params[3]);
		}
		else
		{
			send_err(user, ERR_ALREADYREGISTRED(user.get_nickname()));
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
	(void)user, (void)params;
 	//int user_idx = find_user_idx(user.get_fd());

 	//if (params.size() != 2)
 	//	send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "OPER"));
 	//if (params[0] != SUPER_NICK)
 	//	send_err(user, ERR_WRONGUSERNAME, "wrong host nick name");
 	//if (params[1] != SUPER_PASS)
 	//	send_err(user, ERR_PASSWDMISMATCH, "wrong host password");
 	//_users[user_idx].set_admin(true);
 	//send_msg(user, RPL_YOUREOPER, "Operator privileges have been obtained");
 	//std::cout << "\nUSER[" << user_idx << "] Operator privileges have been obtained\n" << std::endl;
}

void	Server::cmd_mode(User &user, std::vector<std::string> params)
{
	(void)user, (void)params;
 	//int user_idx = find_user_idx(user.get_fd());

 	//if (!_users[user_idx].is_admin())
 	//	send_err(user, ERR_NOPRIVILEGES, "you're not operator");
 	//if (params[0].empty() || params[1].empty())
 	//	send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "MODE"));
 	//if ((params[0][0] == '+' || params[0][0] == '-' ) && params[0][1] == 'o' && params[0].size() == 2)
 	//{
 	//	if (find_nickname(params[1]) == -1)
 	//		send_err(user, ERR_NOSUCHNICK(user.get_nickname()));
 	//	if (params[0][0] == '+')
 	//		_users[find_nickname(params[1])].set_admin(true);
 	//	else
 	//		_users[find_nickname(params[1])].set_admin(false);
 	//}
 	//else
 	//	send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "MODE"));
}

void	Server::cmd_join(User &user, std::vector<std::string> params) // o.k
{
	if (params.size() < 1)
		send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "JOIN"));

	if (is_valid_room_name(params[0]) == false)
		send_err(user, ERR_NOSUCHCHANNEL(user.get_nickname(), params[0]));
	int i = find_room_idx(params[0]);
	if (i == -1) // 방이 없을 때
	{
		if (user.get_rooms().size() >= MAX_ROOM_USER)
			send_err(user, ERR_TOOMANYCHANNELS(user.get_nickname(), params[0]));

		Room room(params[0]);
		if (params[1].empty() == false)
			room.set_key(params[1]);
		room.add_user(user);
		_rooms.push_back(room);
		std::cout << "Room " << params[0] << " created" << std::endl;
		user.add_room(&(_rooms.back()));
		room.send_all(":" + user.get_nickname() + " JOIN " + room.get_name() + "\n");
		send_msg(user, RPL_NOTOPIC(user.get_nickname(), room.get_name()));
		send_msg(user, RPL_NAMREPLY(user.get_nickname(), room.get_name(), room.get_user_list()));
		send_msg(user, RPL_ENDOFNAMES(user.get_nickname(), room.get_name()));
		}
	else
	{
		if (user.get_room(params[0]) != -1)
			return ;
		if (params[1].empty() == false && _rooms[i].get_key() != "" &&_rooms[i].get_key() != params[1])
			send_err(user, ERR_BADCHANNELKEY(user.get_nickname(), _rooms[i].get_name()));
		if (_rooms[i].get_users().size() > 10)
			send_err(user, ERR_CHANNELISFULL(user.get_nickname(), _rooms[i].get_name()));
		user.add_room(&_rooms[i]);
		_rooms[i].add_user(user);
		_rooms[i].send_all(":" + user.get_nickname() + " JOIN " + _rooms[i].get_name() + "\n");
		if (_rooms[i].get_topic() == "")
			send_msg(user, RPL_NOTOPIC(user.get_nickname(), _rooms[i].get_name()));
		else
			send_msg(user, RPL_TOPIC(user.get_nickname(), _rooms[i].get_name(), _rooms[i].get_topic()));
		send_msg(user, RPL_NAMREPLY(user.get_nickname(), _rooms[i].get_name(), _rooms[i].get_user_list()));
		send_msg(user, RPL_ENDOFNAMES(user.get_nickname(), _rooms[i].get_name()));
	}
}

void	Server::cmd_list(User &user, std::vector<std::string> params)
{
	(void)user, (void)params;
	//send_msg(user, RPL_LISTSTART(user.get_nickname()));
	//if (params.size() == 0)
	//{
	//	for(unsigned long i = 0; i < _rooms.size(); i++)
	//	{
	//		std::stringstream	tmp;
	//		tmp << _params[0].get_users().size();
	//		send_msg(user, RPL_LIST(user.get_nickname(), _params[0].get_name(), tmp.str(), _params[0].get_topic()));
	//	}
	//}
	//else if (params.size() == 1)
	//{
	//	std::vector<std::string>	rooms = split(params[0], ',');
	//	for(unsigned long i = 0; i < rooms.size(); i++)
	//	{
	//		if (find_room_idx(params[0]) == -1)
	//			send_err(user, ERR_NOSUCHCHANNEL(user.get_nickname(), params[0]));
	//		else
	//		{
	//			std::stringstream	tmp;
	//			tmp << _rooms[find_room_idx(params[0])].get_users().size();
	//			send_msg(user, RPL_LIST(user.get_nickname(), _rooms[find_room_idx(params[0])].get_name(), tmp.str(), _rooms[find_room_idx(params[0])].get_topic()));
	//		}
	//	}
	//}
	//send_msg(user, RPL_LISTEND(user.get_nickname()));
}

void	Server::cmd_kick(User &user, std::vector<std::string> params) // o.k
{
 	if (params.size() < 2)
 		send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "KICK"));

	int room_idx = find_room_idx(params[0]);
	if (room_idx == -1)
		send_err(user, ERR_NOSUCHCHANNEL(user.get_nickname(), params[0]));
	if (!_rooms[room_idx].is_admin(user.get_nickname()))
		send_err(user, ERR_NOPRIVILEGES(user.get_nickname()));
	if (!_rooms[room_idx].is_admin(user.get_nickname()))
		send_err(user, ERR_NOTONCHANNEL(user.get_nickname(), _rooms[room_idx].get_name()));
	if (_rooms[room_idx].is_user(params[1]) == false)
		send_err(user, ERR_NOSUCHNICK(user.get_nickname()));

	_rooms[room_idx].remove_user(params[1]);
	_rooms[room_idx].send_all(":" + user.get_nickname() + " KICK " + params[0] + " " + params[1] + "\n");
	user.delete_room(params[0]);
}

void	Server::cmd_part(User &user, std::string param) // o.k
{
	if (param.empty())
		send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "PART"));

	Room &room = _rooms[find_room_idx(param)];

	if(find_room_idx(param) == -1)
		send_err(user, ERR_NOSUCHCHANNEL(user.get_nickname(), param));
	if(!room.is_user(user.get_nickname()))
		send_err(user, ERR_NOTONCHANNEL(user.get_nickname(), param));

	user.delete_room(param);
	room.send_all(":" + user.get_nickname() + " PART " + room.get_name() + "\n");
	room.remove_user(user.get_nickname());
	if (room.get_users().size() == 0)
		_rooms.erase(_rooms.begin() + find_room_idx(param));
}

void	Server::cmd_privmsg(User &user, std::vector<std::string> params) // o.k
{
	if (!params.size() || params[0][0] == ':')
		send_err(user, ERR_NORECIPIENT(user.get_nickname(), params[0]));
	if (params.size() < 2)
		send_err(user, ERR_NOTEXTTOSEND(user.get_nickname()));

	if  (params[0][0] == '#') // 방에서 메시지를 보낼 때
	{
		if (find_room_idx(params[0]) == -1)
			send_err(user, ERR_CANNOTSENDTOCHAN(user.get_nickname(), params[0]));
		else
			send_privmsg_to_room(user, find_room_idx(params[0]), params[1]);
	}
	else // 유저에게 메시지를 보낼 때
	{
		if (find_nickname(params[0]) == -1)
			send_err(user, ERR_NOSUCHNICK(user.get_nickname()));
		else
			send_privmsg(_users[find_nickname(params[0])], user, params[1]);
	}
}

void	Server::cmd_notice(User &user, std::vector<std::string> params) // o.k
{
	if (!params.size() || params[0][0] == ':')
		send_err(user, ERR_NORECIPIENT(user.get_nickname(), params[0]));
	if (params.size() < 2)
		send_err(user, ERR_NOTEXTTOSEND(user.get_nickname()));

	if (find_nickname(params[0]) == -1)
		send_err(user, ERR_NOSUCHNICK(user.get_nickname()));
	else
		send_notice(_users[find_nickname(params[0])], user, params[1]);
}

void	Server::cmd_names(User &user, std::vector<std::string> params)
{
	std::string msg = " :";

	if (params.size() > 1)
		send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "NAMES"));
	else if (params.size() == 0)
	{
		for (size_t i = 0; i < _rooms.size(); i++)
		{
			send_msg(user, RPL_NAMREPLY(user.get_nickname(), _rooms[i].get_name(), _rooms[i].get_user_list()));
			send_msg(user, RPL_ENDOFNAMES(user.get_nickname(), _rooms[i].get_name()));
		}
		send_msg(user, RPL_NAMREPLY(user.get_nickname(), "*", get_wait_list()));
		send_msg(user, RPL_ENDOFNAMES(user.get_nickname(), "*"));
	}
	else if (params.size() == 1)
	{
		int i = find_room_idx(params[0]);
			send_msg(user, RPL_NAMREPLY(user.get_nickname(), _rooms[i].get_name(), _rooms[i].get_user_list()));
			send_msg(user, RPL_ENDOFNAMES(user.get_nickname(), _rooms[i].get_name()));
	}
}

void	Server::cmd_topic(User &user, std::vector<std::string> params)
{
	int server_room = -1;
	int user_room = -1;

	if  (params.empty() || (params.size() != 1 && params.size() != 2))
		send_err(user, ERR_NEEDMOREPARAMS(user.get_nickname(), "TOPIC"));
	if ((server_room = find_room_idx(params[0])))
		send_err(user, ERR_NOSUCHCHANNEL(user.get_nickname(), params[0]));
	if (params.size() == 1)
	{
		if (_rooms[server_room].get_topic().empty())
			send_msg(user, RPL_NOTOPIC(user.get_nickname(), params[0]));
		else
			send_msg(user, RPL_TOPIC(user.get_nickname(), params[0], _rooms[server_room].get_topic()));
	}
	else
	{
		if ((user_room = user.get_room(params[0])) == -1)
			send_err(user, ERR_NOTONCHANNEL(user.get_nickname(), params[0]));
		if ((_rooms[server_room].is_admin(user.get_nickname())) == false)
			send_err(user, ERR_CHANOPRIVSNEEDED(user.get_nickname(), params[0]));
		user.get_rooms()[user_room]->set_topic(params[1]);
		user.get_rooms()[user_room]->send_all(RPL_SETTOPIC(params[0], params[1]));
		user.get_rooms()[user_room]->send_all(RPL_TOPIC(user.get_nickname(), params[0], _rooms[server_room].get_topic()));
	}
}

void	Server::quit(User &user)
{
	if (user.get_rooms().empty() == false)
		user.get_rooms().clear();
	send_msg(user, RPL_NONE((std::string)"Goodbye!"));
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
	send_err(user, ERR_FLOOD(ss.str()));
	return true;
}

void	Server::cmd_ping(User &user, const Message &msg)
{
	std::string name = SERV;
	if (msg.get_params().size() == 0)
		send_err(user, ERR_NOORIGIN);
	else
		send_pong(user, msg.get_params()[0]);
}

void	Server::cmd_pong(User &user, const Message &msg)
{
	if (msg.get_params().size() <= 0 || msg.get_params()[0] != SERV)
	{
		if (msg.get_params().size() > 0)
			send_err(user, ERR_NOSUCHSERVER(msg.get_params()[0]));
		else
			send_err(user, ERR_NOSUCHSERVER((std::string)""));
	}
}

bool	Server::is_valid_room_name(const std::string &name)
{
	if (name.length() > 50 || name.find_first_of("&#+!") != 0)
		return false;
	for (unsigned int i = 0; i < name.length(); i++) {
		if (isspace(name[i]) || iscntrl(name[i]) || name[i] == ',')
			return false;
	}
	return true;
}
