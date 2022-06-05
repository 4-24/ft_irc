# include "User.hpp"

User::User(int fd) { _fd = fd; }

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

int	User::get_fd() const
{
	return (_fd);
}

Message	User::get_message() const
{
	return (_message);
}
