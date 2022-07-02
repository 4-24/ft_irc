# include "User.hpp"

User::User() {}

User::User(int fd)
{
	_fd = fd;
	_is_registered = false;
	_is_authenticated = false;
	_is_admin = false;
	_last_message_time = time(NULL);
	_message_timeout = 1;
	_nickname = "";
	_username = "";
	_realname = "";
}

User::User(const User& user)
{
	_fd = user._fd;
	_is_registered = user._is_registered;
	_is_authenticated = user._is_authenticated;
	_is_admin = user._is_admin;
	_last_message_time = user._last_message_time;
	_message_timeout = user._message_timeout;
	_nickname = user._nickname;
	_username = user._username;
	_realname = user._realname;
	_message = user._message;
	_buffer = user._buffer;
}

User&	User::operator=(const User &user)
{
	_fd = user._fd;
	_is_registered = user._is_registered;
	_is_authenticated = user._is_authenticated;
	_is_admin = user._is_admin;
	_last_message_time = user._last_message_time;
	_message_timeout = user._message_timeout;
	_nickname = user._nickname;
	_username = user._username;
	_realname = user._realname;
	_message = user._message;
	_buffer = user._buffer;
	return (*this);
}

User::~User() {}

void	User::add_buffer(std::string message)
{
	_buffer += message;

	std::cout << "------------------------------------" << std::endl;
	std::cout << "Message from User " << _fd << std::endl;
	std::cout << "------------------------------------" << std::endl;
	std::cout << "message: " << _buffer << std::endl;
	std::cout << "------------------------------------" << std::endl;
}

void	User::clear_message()
{
	_message = Message();
}

void	User::setup_message()
{
	_message.setup(_buffer);
	size_t pos = _buffer.find_first_of("\r\n");
	if (pos != std::string::npos && _buffer[pos] == '\n' && _buffer[pos + 1] == '\0')
		_buffer.clear();
	else if (pos != std::string::npos && _buffer[pos + 2] != '\0')
			_buffer = _buffer.substr(pos + 2);
	else if (!_buffer.empty())
		_buffer.clear();
	if (!_message.params().empty())
	{
		std::cout << "params: ";
		for (unsigned int i = 0; i < _message.params().size(); i++)
			std::cout << _message.params()[i] << " ";
		std::cout << std::endl;
	}
}

bool	User::is_registered()
{
	return _is_registered;
}

bool	User::is_authenticated()
{
	return _is_authenticated;
}

bool	User::is_admin()
{
	return _is_admin;
}

std::string	User::buffer() const
{
	return _buffer;
}

std::string User::nickname() const
{
	std::string new_nickname  = _nickname;
	return new_nickname;
}
std::string	User::username() const
{
	return _username;
}

std::string	User::realname() const
{
	return _realname;
}

int	User::fd() const
{
	return (_fd);
}

Message	User::message() const
{
	return (_message);
}

std::string	User::prefix() const
{
	return _message.prefix();
}

time_t	User::last_message_time() const
{
	return _last_message_time;
}

time_t	User::message_timeout() const
{
	return _message_timeout;
}

std::string User::fullname() const
{
	return std::string(":" + _nickname + "!" + _username + "@" + _realname);
}

bool	User::is_admin() const
{
	return _is_admin;
}

void	User::set_fd(int fd)
{
	_fd = fd;
}

void	User::set_nickname(std::string nickname)
{
	_nickname = nickname;
}

void	User::set_username(std::string username)
{
	_username = username;
}

void	User::set_realname(std::string realname)
{
	_realname = realname;
}

void	User::set_authenticated(bool authenticated)
{
	_is_authenticated = authenticated;
}

void	User::set_registered(bool registered)
{
	_is_registered = registered;
}

void	User::set_last_message_time(time_t last_message_time)
{
	_last_message_time = last_message_time;
}

void	User::set_message_timeout(time_t message_timeout)
{
	_message_timeout = message_timeout;
}

void	User::set_admin(bool admin)
{
	_is_admin = admin;
}

std::set<std::string>& User::rooms(void)
{
	return _rooms;
}

void User::send_msg(std::string msg, int flag)
{
	send(_fd, msg.c_str(), msg.size(), flag);
}
void	User::send_err(std::string msg, int flag)
{
	send(_fd, msg.c_str(), msg.size(), flag);
	throw std::runtime_error(msg.c_str());
}
