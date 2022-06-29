# include "Reply.hpp"
# include "Server.hpp"

void	Server::send_motd(User user)
{
	user.send_msg(RPL_MOTDSTART(user.nickname()));
	user.send_msg(RPL_MOTD(user.nickname(), (std::string)"*** 42Seoul 4-24 network ***"));
	user.send_msg(RPL_ENDOFMOTD(user.nickname()));
}

void	Server::send_pong(User &user, std::string msg)
{
	std::string serv_name = SERV;
	std::string res = ":" + serv_name + " PONG :" + msg + "\n";
	user.send_msg(res);
}
