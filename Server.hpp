#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include <exception>
# include <cstring>
# include <cerrno>
# include <unistd.h>

# include "User.hpp"
# include "Socket.hpp"
# include "Message.hpp"

# define TIMEOUT 1000
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define RESET "\x1b[0m"
# define MSG_LEN 512


class Server
{
	private:
		int								_port;
		int								server_socket;
		std::string						_password;
		std::vector<User *>				_users;
		Socket							*_socket;
		std::vector<struct pollfd>		_fds;

	public:
		Server(int port);
		Server(int port, std::string password);
		~Server();
		void							start();
		void							create_poll(int fd, bool is_server);
		User							&find_user(int fd);
		int								find_user_idx(int fd);
		void							chat(User &user);
		void							execute(User &user, Message message);

		void							send_msg(int fd, std::string message);
		void							send_err(int fd, std::string error);

		void	cmd_pass(User &user, std::vector<std::string> params);
		void	cmd_nick(int user_fd, std::vector<std::string> params);
		void	cmd_user(int user_fd, std::vector<std::string> params);
		void	cmd_oper(int user_fd, std::vector<std::string> params);
		void	cmd_mode(int user_fd, std::vector<std::string> params);
		void	cmd_join(int user_fd, std::vector<std::string> params);
		void	cmd_kick(int user_fd, std::vector<std::string> params);
		void	cmd_part(int user_fd, std::vector<std::string> params);
		void	cmd_names(int user_fd, std::vector<std::string> params);
		void	cmd_privmsg(int user_fd, std::vector<std::string> params);
		void	cmd_notice(int user_fd, std::vector<std::string> params);

		void	quit(int user_fd);
};

#endif

