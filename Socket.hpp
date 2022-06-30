#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <poll.h>
# include <arpa/inet.h>
# include <string>
# include <stdlib.h>
# include <iostream>
# include <sstream>
# include <exception>
# include <cstring>
# include <cerrno>
# include <fcntl.h>

# define BACKLOG 10


class Socket
{
	private:
		std::string						_host;
		int								_port;
		int								_listen;
		int								_connect;

		struct addrinfo					*_info;

	public:
		Socket(int port);
		~Socket();
		void							setup();
		void							create();
		void							allow();

		int								ft_connect();
		int								ft_listen();
};

#endif
