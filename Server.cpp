# include "Server.hpp"

Server::Server(int port) {
	_port = port;
	_socket = new Socket(port);
}

Server::~Server() {}

void	Server::start()
{
	_socket->create();
	create_poll(_socket->get_listen(), true);

	while (1)
	{
		if (int res = poll(_fds.data(), _fds.size(), TIMEOUT))
		{
			if (_fds[0].revents & POLLIN) // 서버 소켓에서 POLLIN 이벤트가 발생한 경우
			{
				_socket->allow();
				create_poll(_socket->get_connect(), false);
				std::cout << "User connected: " << _socket->get_connect() << std::endl;
			}
			for (size_t i = 1; i < _fds.size(); i++)
			{
				if (_fds[i].revents & POLLIN) // 클라이언트 소켓에서 POLLIN 이벤트가 발생한 경우
				{
					int fd = _fds[i].fd;
					User &user = find_user(fd);
					chat(user);
					_fds[i].revents = 0;
				}
			}
		}
	}
}

void	Server::create_poll(int fd, bool is_server)
{
	struct pollfd pf;

	pf.fd = fd;
	pf.events = POLLIN;
	pf.revents = 0;
	_fds.push_back(pf);

	if (!is_server)
		_users.push_back(new User(fd));
}

User &Server::find_user(int fd)
{
	for (size_t i = 1; i < _users.size(); i++)
		if (_users[i]->get_fd() == fd)
			return *_users[i];

	return *_users[0];
}

void	Server::chat(User &user)
{
	std::string info;
	char buff[100];

	std::memset(buff, 0, sizeof buff);
	int nbytes;
	if ((nbytes = recv(user.get_fd(), buff, sizeof buff, 0)) < 0)
		throw std::runtime_error("recv");
	else
	{
		buff[nbytes] = 0;
		info += buff;
		if (info.find("\r\n") != std::string::npos)
		{
			std::cout << info << std::endl;
			user.send_message(info);
			//TODO: execute command 구현
			info.clear();
		}
	}
}
