# include "Server.hpp"

Server::Server(int port) {
	_port = port;
	_password = "";
	_socket = new Socket(port);
}

Server::Server(int port, std::string password) {
	_port = port;
	_password = password;
	_socket = new Socket(port);
}

Server::~Server()
{
	delete _socket;
}

void	Server::start()
{
	_socket->create();
	create_poll(_socket->ft_listen(), true);

	while (1)
	{
		if (poll(_fds.data(), _fds.size(), TIMEOUT))
		{
			if (_fds[0].revents & POLLIN) // 서버 소켓에서 POLLIN 이벤트가 발생한 경우
			{
				_socket->allow();
				create_poll(_socket->ft_connect(), false);
				std::cout << "User connected: " << _socket->ft_connect() << std::endl;
			}
			for (unsigned int i = 1; i < _fds.size(); i++)
			{
				if (_fds[i].revents & POLLIN) // 클라이언트 소켓에서 POLLIN 이벤트가 발생한 경우
				{
					int fd = _fds[i].fd;
					chat(fd);
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
	{
		_nicks[fd] = anonym(fd);
		User user(fd);
		_users[anonym(fd)] = user;
		if (_password != "")
			user.send_msg(RPL_NONE((std::string)"Please enter ircserv password."));
	}
}

std::string	Server::wait_list()
{
	std::stringstream ss;
	ss.str("");

	if (_users.size() > 0)
	{
		for (std::map<std::string, User>::iterator itr = _users.begin(); itr != _users.end(); ++itr)
		{
			if (_users[itr->first].rooms().size() == 0)
				ss << _users[itr->first].nickname() + " ";
		}
	}
	return ss.str();
}

void	Server::chat(int fd)
{
	char		buff[MSG_LEN];
	int			nbytes;

	std::memset(buff, 0, sizeof buff);
	if ((nbytes = recv(_users[_nicks[fd]].fd(), buff, sizeof buff, 0)) <= 0 || (nbytes > MSG_LEN))
	{
		if (nbytes < 0)
			throw std::runtime_error("recv: recv error");
		else if (nbytes > MSG_LEN)
			throw std::runtime_error("recv: message too long");
		else if (nbytes == 0)
			quit(_users[_nicks[fd]]);
	}
	else
	{
		buff[nbytes] = 0;
		_users[_nicks[fd]].add_buffer(buff);
		while (_users[_nicks[fd]].buffer().find_first_of("\r\n") != std::string::npos)
		{
			while (_users[_nicks[fd]].buffer().size() > 0)
			{
				_users[_nicks[fd]].setup_message();
				execute(_users[_nicks[fd]], _users[_nicks[fd]].message());
				_users[_nicks[fd]].clear_message();
			}
		}
	}
}

bool	Server::is_room(std::string name)
{
	return (_rooms.find(name) != _rooms.end());
}

bool	Server::is_user(std::string name)
{
	return (_users.find(name) != _users.end());
}

bool	Server::is_nick(int i)
{
	return (_nicks.find(i) != _nicks.end());
}

std::string Server::anonym(int i)
{
	std::stringstream s_fd;
	s_fd << i;
	std::string name = "anonym" + s_fd.str();
	return name;
}

std::vector<std::string>	Server::split(std::string input, char delimiter) {
	std::vector<std::string>	result;
	std::stringstream			ss(input);
	std::string					tmp;

	while (getline(ss, tmp, delimiter)) result.push_back(tmp);
	return result;
}
