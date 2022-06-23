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
	std::string res = header(code, user) + message + "\n";
	send(user.get_fd(), res.c_str(), res.size(), 0);
}

void	Server::send_motd(User user)
{
	std::string serv_name = SERV;
	std::string res1 = ":- " + serv_name + " Message of the day -";
	send_msg(user, RPL_MOTDSTART, res1);

	std::string res2 = "Welcome to the Internet Relay Network:= " + user.prefix();
	send_msg(user, RPL_MOTD, res2);
	send_msg(user, RPL_ENDOFMOTD, ":End of /MOTD command");
}

void	Server::send_err(User user, int code, std::string message)
{
	std::string res = header(code, user) + message + "\n";
	send(user.get_fd(), res.c_str(), res.size(), 0);
	throw std::runtime_error((message + "\n").c_str());
}

void	Server::send_privmsg_to_room(User &from, int idx, std::string message)
{
	for (size_t i = 0; i < _rooms[idx].get_users().size(); i++)
		if (_rooms[idx].get_users()[i]->get_fd() != from.get_fd())
			send_privmsg(*(_rooms[idx].get_users()[i]), from, message);
}

void	Server::send_notice_to_room(User &from, int idx, std::string message)
{
	for (size_t i = 0; i < _rooms[idx].get_users().size(); i++)
		if (_rooms[idx].get_users()[i]->get_fd() != from.get_fd())
			send_notice(*(_rooms[idx].get_users()[i]), from, message);
}

void	Server::send_privmsg(User &to, User &from, std::string msg)
{
	std::string res = from.prefix() + " PRIVMSG " + from.get_nickname() + " " + ":" + msg + "\n";
	send(to.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}

void	Server::send_notice(User &to, User &from, std::string msg)
{
	std::string res = from.prefix() + " NOTICE " + from.get_nickname() + " " + ":" + msg + "\n";
	send(to.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}

void	Server::send_pong(User &user, std::string msg)
{
	std::string serv_name = SERV;
	std::string res = ":" + serv_name + " PONG :" + msg + "\n";
	send(user.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}
