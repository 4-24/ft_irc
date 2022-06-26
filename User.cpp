# include "User.hpp"

User::User(int fd)
{
	_fd = fd;
	_is_registered = false;
	_is_authenticated = false;
	_is_admin = false;
	_last_message_time = time(NULL);
	_message_timeout = 1;
	_room_count = 0;
	_nickname = "";
	_username = "";
	_realname = "";
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
	_buffer.clear();
	_message = Message();
}

void	User::setup_message()
{
	_buffer.replace(_buffer.find("\n"), 2, "\r\n");
	_message.setup(_buffer);

	std::cout << "prefix: " << _message.get_prefix() << std::endl;
	std::cout << "command: " << _message.get_command() << std::endl;
	if (_message.get_params().size() > 0)
	{
		std::cout << "params: ";
		for (unsigned long i = 0; i < _message.get_params().size(); i++)
			std::cout << _message.get_params()[i] << " ";
		std::cout << std::endl;
	}
}

void	User::up_room_count()
{
	_room_count++;
}

void	User::down_room_count()
{
	_room_count--;
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

std::string	User::get_buffer() const
{
	return _buffer;
}

std::string User::get_nickname() const
{
	return _nickname;
}

std::string	User::get_username() const
{
	return _username;
}

std::string	User::get_realname() const
{
	return _realname;
}

int	User::get_fd() const
{
	return (_fd);
}

Message	User::get_message() const
{
	return (_message);
}

std::string	User::get_prefix() const
{
	return _message.get_prefix();
}

time_t	User::get_last_message_time() const
{
	return _last_message_time;
}

time_t	User::get_message_timeout() const
{
	return _message_timeout;
}

std::string User::prefix() const
{
	return std::string(_nickname + "!" + _username + "@" + _realname);
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

int	User::get_room_count() const
{
	return _room_count;
}
