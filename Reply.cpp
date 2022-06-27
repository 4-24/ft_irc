# include "Reply.hpp"
# include "Server.hpp"


void	Server::send_msg(User user, std::string message)
{
	send(user.get_fd(), message.c_str(), message.size(), 0);
}

void	Server::send_motd(User user)
{
	send_msg(user, RPL_MOTDSTART(user.get_nickname()));
	send_msg(user, RPL_MOTD(user.get_nickname(), (std::string)"*** 42Seoul 4-24 network ***"));
	send_msg(user, RPL_ENDOFMOTD(user.get_nickname()));
}

void	Server::send_err(User user, std::string message)
{
	send(user.get_fd(), message.c_str(), message.size(), 0);
	throw std::runtime_error((message + "\n").c_str());
}

void	Server::send_privmsg_to_room(User &from, int idx, std::string message)
{
	for (unsigned int i = 0; i < _rooms[idx].get_users().size(); i++)
		if (_rooms[idx].get_users()[i]->get_fd() != from.get_fd())
			send_privmsg(*(_rooms[idx].get_users()[i]), from, message);
}

void	Server::send_notice_to_room(User &from, int idx, std::string message)
{
	for (unsigned int i = 0; i < _rooms[idx].get_users().size(); i++)
		if (_rooms[idx].get_users()[i]->get_fd() != from.get_fd())
			send_notice(*(_rooms[idx].get_users()[i]), from, message);
}

void	Server::send_privmsg(User &to, User &from, std::string msg)
{
	std::string res = ":" + from.prefix() + " PRIVMSG " + from.get_nickname() + " " + ":" + msg + "\n";
	send(to.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}

void	Server::send_notice(User &to, User &from, std::string msg)
{
	std::string res = ":" + from.prefix() + " NOTICE " + from.get_nickname() + " " + ":" + msg + "\n";
	send(to.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}

void	Server::send_pong(User &user, std::string msg)
{
	std::string serv_name = SERV;
	std::string res = ":" + serv_name + " PONG :" + msg + "\n";
	send(user.get_fd(), res.c_str(), res.size(), SO_NOSIGPIPE);
}
