#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <string>
# include <vector>
# include <queue>
# include <iostream>
# include <sstream>
# include <exception>
# include <cstring>
# include <cerrno>
# include <unistd.h>

# include "User.hpp"
# include "Socket.hpp"
# include "Message.hpp"
# include "Room.hpp"
# include "Reply.hpp"

# define TIMEOUT 1000
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define DIM "\x1b[2m"
# define RESET "\x1b[0m"
# define MSG_LEN 1024
# define SUPER_NICK "klp"
# define SUPER_PASS "424"
# define SERV "ircserv"

class Server
{
	private:
		int								_port;
		int								server_socket;
		std::string						_password;
		std::map<std::string, User>		_users;
		std::map<int, std::string>		_nicks;
		std::map<std::string, Room>		_rooms;
		Socket							*_socket;
		std::vector<struct pollfd>		_fds;

	public:
		Server(int port);
		Server(int port, std::string password);
		~Server();
		void							start();
		void							create_poll(int fd, bool is_server);
		void							chat(int fd);
		void							execute(User &user, Message message);
		bool							is_room(std::string name);
		bool							is_user(std::string name);
		bool							is_nick(int i);
		bool							is_flooding(User &user);
		bool							is_user_fd(int i);
		std::string						wait_list();
		bool							is_valid_room_name(const std::string &name);
		std::vector<std::string>		split(std::string input, char delimiter);
		std::string						anonym(int i);

		void	cmd_pass(User &user, std::vector<std::string> &params);
		void	cmd_nick(User &user, std::vector<std::string> &params);
		void	cmd_user(User &user, std::vector<std::string> &params);
		void	cmd_oper(User &user, std::vector<std::string> &params);
		void	cmd_mode(User &user, std::vector<std::string> &params);
		void	cmd_join(User &user, std::vector<std::string> &params);
		void	cmd_kick(User &user, std::vector<std::string> &params);
		void	cmd_part(User &user, std::vector<std::string> &params);
		void	cmd_privmsg(User &user, std::vector<std::string> &params);
		void	cmd_notice(User &user, std::vector<std::string> &params);
		void	cmd_names(User &user, std::vector<std::string> &params);
		void	cmd_list(User &user, std::vector<std::string> &params);
		void	cmd_topic(User &user, std::vector<std::string> &params);

		void	quit(User &user);
		void	cmd_ping(User &user, const Message &msg);
		void	cmd_pong(User &user, const Message &msg);

		void							send_motd(User user);
		void							send_pong(User &user, std::string msg);
};

#endif
