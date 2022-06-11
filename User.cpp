# include "User.hpp"

User::User(int fd)
{
	_fd = fd;
	_is_registered = false;
	_is_authenticated = false;
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

bool	User::is_registered()
{
	return _is_registered;
}

bool	User::is_authenticated()
{
	return _is_authenticated;
}

std::string	User::get_buffer() const
{
	return _buffer;
}

int	User::get_fd() const
{
	return (_fd);
}

Message	User::get_message() const
{
	return (_message);
}

void	User::set_authenticated(bool authenticated)
{
	_is_authenticated = authenticated;
}

void	User::set_registered(bool registered)
{
	_is_registered = registered;
}
