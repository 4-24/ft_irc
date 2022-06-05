# include "User.hpp"

User::User(int fd)
{
	_fd = fd;
	_is_registered = false;
	_is_authenticated = false;
}

User::~User() {}

void	User::send_message(std::string message)
{
	_message.setup(message);

	std::cout << "------------------------------------" << std::endl;
	std::cout << "Message from User " << _fd << std::endl;
	std::cout << "------------------------------------" << std::endl;
	std::cout << "message: " << message;
	std::cout << "prefix: " << _message.get_prefix() << std::endl;
	std::cout << "command: " << _message.get_command() << std::endl;
	std::cout << "parameter[0]: " << *(_message.get_params().begin()) << std::endl;
}

bool	User::is_registered()
{
	return _is_registered;
}

bool	User::is_authenticated()
{
	return _is_authenticated;
}

int	User::get_fd() const
{
	return (_fd);
}

Message	User::get_message() const
{
	return (_message);
}
