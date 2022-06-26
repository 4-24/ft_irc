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
	{
		_users.push_back(User(fd));
		if (_password != "")
			send_msg(_users.back(), RPL_NONE((std::string)"Please enter ircserv password."));
	}
}

User &Server::find_user(int fd)
{
	for (size_t i = 0; i < _users.size(); i++)
		if (_users[i].get_fd() == fd)
			return _users[i];

	return _users[0];
}

int	Server::find_user_idx(int fd)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i].get_fd() == fd)
			return i;
	}

	return -1;
}

int	Server::find_fd_idx(int fd)
{
	for (size_t i = 1; i < _fds.size(); i++)
		if (_fds[i].fd == fd)
			return i;

	return -1;
}

std::string	Server::get_wait_list()
{
	std::stringstream ss;
	ss.str("");

	if (_users.size() > 0)
	{
		for (unsigned long i = 0; i < _users.size(); i++)
		{
			if (_users[i].get_room_count() == 0)
				ss << _users[i].get_nickname() << " ";
		}
	}
	return ss.str();
}

void	Server::chat(User &user)
{
	char		buff[MSG_LEN];
	int			nbytes;

	std::memset(buff, 0, sizeof buff);
	if ((nbytes = recv(user.get_fd(), buff, sizeof buff, 0)) <= 0 || (nbytes > MSG_LEN))
	{
		if (nbytes < 0)
			throw std::runtime_error("recv: recv error");
		else if (nbytes > MSG_LEN)
			throw std::runtime_error("recv: message too long");
		else if (nbytes == 0)
			quit(user);
	}
	else
	{
		buff[nbytes] = 0;
		user.add_buffer(buff);
		if (user.get_buffer().find_first_of("\n") != std::string::npos)
		{
			user.setup_message();
			execute(user, user.get_message());
			user.clear_message();
		}
	}

}

int	Server::find_room_idx(std::string name)
{
	for (size_t i = 0; i < _rooms.size(); i++)
		if (_rooms[i].get_name() == name)
			return i;

	return -1;
}

int	Server::find_nickname(std::string name)
{
	for (size_t i = 0; i < _users.size(); i++)
		if (_users[i].get_nickname() == name)
			return i;
	return -1;
}

int	Server::find_username(std::string name)
{
	for (size_t i = 0; i < _users.size(); i++)
		if (_users[i].get_username() == name)
			return i;

	return -1;
}

std::vector<std::string>	Server::split(std::string input, char delimiter) {
	std::vector<std::string>	result;
	std::stringstream			ss(input);
	std::string					tmp;

	while (getline(ss, tmp, delimiter)) result.push_back(tmp);
	return result;
}
