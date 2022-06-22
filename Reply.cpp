# include "Reply.hpp"
# include "Server.hpp"

std::string Server::header(int code, User &user)
{
	std::string msg = ":ircserv ";
	std::stringstream ss;
	ss << code;
	msg += ss.str() + " " + user.get_nickname() + " ";
	return msg;
}

void	Server::send_msg(User user, int code, std::string message)
{
	std::string res = header(code, user) + ": " + message + "\n";
	send(user.get_fd(), res.c_str(), res.size(), 0);
}

void	Server::send_user_info(User user, std::string msg)
{
	std::stringstream ss;
	ss << "[" << user.get_nickname() << "!" << user.get_username() << "@" << "irc.4-24.kr]";
	send_msg(user, 123, msg + ss.str());
}

void	Server::send_err(User user, int code, std::string message)
{
	std::string res = header(code, user) + ": " + message + "\n";
	send(user.get_fd(), res.c_str(), res.size(), 0);
	throw std::runtime_error((message + "\n").c_str());
}

void	Server::send_privmsg_to_room(int sender, int idx, std::string message)
{
	for (size_t i = 0; i < _rooms[idx].get_users().size(); i++)
		if (_rooms[idx].get_users()[i]->get_fd() != sender)
			send_privmsg(*(_rooms[idx].get_users()[i]), message);
}

void	Server::send_notice_to_room(int sender, int idx, std::string message)
{
	for (size_t i = 0; i < _rooms[idx].get_users().size(); i++)
		if (_rooms[idx].get_users()[i]->get_fd() != sender)
			send_notice(*(_rooms[idx].get_users()[i]), message);
}

void	Server::send_privmsg(User &user, std::string msg)
{
	std::string res = ":" + user.prefix() + " "
			+ "PRIVMSG " + user.get_nickname() + " :" + msg + "\n";
	send(user.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}

void	Server::send_notice(User &user, std::string msg)
{
	std::string res = ":" + user.prefix() + " "
			+ "NOTICE " + user.get_nickname() + " :" + msg + "\n";
	send(user.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}

void	Server::send_pong(User &user, std::string msg)
{
	std::string serv_name = SERV;
	std::string res = ":" + serv_name + " PONG :" + msg + "\n";
	send(user.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}
