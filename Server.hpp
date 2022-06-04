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
# define TIMEOUT 1000


class Server
{
	private:
		int								_port;
		int								server_socket;
		std::vector<User *>				_users;
		Socket							*_socket;
		std::vector<struct pollfd>		_fds;

	public:
		Server(int port);
		~Server();
		void							start();
		void							create_poll(int fd, bool is_server);
		User							&find_user(int fd);
		void							chat(User &user);
};

#endif

