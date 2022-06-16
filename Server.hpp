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
# include "Room.hpp"

# define TIMEOUT 1000
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define RESET "\x1b[0m"
# define MSG_LEN 512
# define SUPER_NICK "klp"
# define SUPER_PASS "424"


class Server
{
	private:
		int								_port;
		int								server_socket;
		std::string						_password;
		std::vector<User>				_users;
		std::vector<Room>				_rooms;
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
		int								find_fd_idx(int fd);
		int								find_nickname(std::string name);
		int								find_username(std::string name);
		void							chat(User &user);
		void							execute(User &user, Message message);
		int								find_room_idx(std::string room_name);
		bool							is_flooding(User &user);
		void							replace_user(User &old_user, User &new_user);

		void							send_msg(int fd, std::string message);
		void							send_err(int fd, std::string error);
		void							send_user_info(User user, std::string msg);
		void							send_channel(int idx, std::string message);

		void	cmd_pass(User &user, std::vector<std::string> params);
		void	cmd_nick(User &user, std::string param);
		void	cmd_user(User &user, std::string param);
		void	cmd_oper(int fd, std::vector<std::string> params);
		void	cmd_mode(int fd, std::vector<std::string> params);
		void	cmd_join(User &user, std::string param);
		void	cmd_kick(int fd, std::vector<std::string> params);
		void	cmd_part(int fd, std::vector<std::string> params);
		void	cmd_names(int fd, std::vector<std::string> params);
		void	cmd_privmsg(int fd, std::vector<std::string> params);

		void	quit(int fd);
};

#endif

