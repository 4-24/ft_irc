# include "Server.hpp"

Server::Server(int port) {
	_port = port;
	_socket = new Socket(port);
}

Server::~Server() {}

void	Server::start()
{
	int status;

	_socket->create();
	while (1)
	{
		if ((status = poll(_user_fds.data(), _user_fds.size(), TIMEOUT)) != -1)
		{
			for (size_t i = 0; i < _user_fds.size(); i++)
			{
				if (_user_fds[i].revents & POLLIN) // 클라이언트 소켓에서 POLLIN 이벤트가 발생한 경우
				{
					int fd = _user_fds[i].fd;
					User &user = find_user(fd);
					chat(user);
					_user_fds[i].revents = 0;
				}
			}
		}
		_socket->allow();
		create_poll(_socket->get_connect());
	}
}

void	Server::create_poll(int connect)
{
	struct pollfd pf;

	if (connect > 0)
	{
		pf.fd = connect;
		pf.events = POLLIN;
		pf.revents = 0;
		_user_fds.push_back(pf);
		_users.push_back(new User(connect));
	}
}

User &Server::find_user(int connect)
{
	for (size_t i = 0; i < _users.size(); i++)
		if (_users[i]->get_fd() == connect)
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
		std::cout << "server: socket " << user.get_fd() << " says " << info << std::endl;
		if (send(user.get_fd(), info.c_str(), info.size(), 0) < 0)
			throw std::runtime_error("send");
		if (info.find("\n\r") != std::string::npos)
		{
			for (size_t i = 0; i < _user_fds.size(); i++)
				if (_user_fds[i].fd != user.get_fd())
					send(_user_fds[i].fd, info.c_str(), info.size(), 0);
			//TODO: execute command 구현
			info.clear();
		}
	}
}
