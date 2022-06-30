# include "Socket.hpp"

Socket::Socket(int port)
{
	this->_host = "localhost";
	this->_port = port;
	this->_listen = -1;
}

Socket::~Socket() {}

void	Socket::setup()
{
	struct addrinfo hints;
	int status;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	std::stringstream ss;
	ss << this->_port;
	std::string str = ss.str();
	const char *port = str.c_str();

	if ((status = getaddrinfo(NULL, port, &hints, &_info)) != 0)
	{
		throw std::runtime_error("getaddrinfo");
		exit(1);
	}
}

void	Socket::create()
{
	setup();

	int yes = 1;
	if ((_listen = socket(_info->ai_family, _info->ai_socktype,
		_info->ai_protocol)) == -1)
	{
		throw std::runtime_error("socket");
		exit(1);
	}
	if (setsockopt(_listen, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
	{
		throw std::runtime_error("reuseaddr");
		exit(2);
	}
	if (fcntl(_listen, F_SETFL, O_NONBLOCK) < 0)
	{
		throw std::runtime_error("fcntl");
		exit(3);
	}
	if (bind(_listen, _info->ai_addr, _info->ai_addrlen) == -1)
	{
		throw std::runtime_error("bind");
		exit(4);
	}
	if (listen(_listen, BACKLOG) == -1)
	{
		throw std::runtime_error("listen");
		exit(5);
	}
}

void	Socket::allow()
{
	struct sockaddr_storage	remoteaddr;
	socklen_t	addrlen = sizeof remoteaddr;
	if ((_connect = accept(_listen, (struct sockaddr *)&remoteaddr,
		&addrlen)) == -1)
		{
			throw std::runtime_error("accept");
			exit(5);
		}
}


int	Socket::ft_connect()
{
	return (_connect);
}

int	Socket::ft_listen()
{
	return (_listen);
}
