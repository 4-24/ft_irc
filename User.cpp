# include "User.hpp"

User::User(int fd) { _fd = fd; }

User::~User() {}

void	User::send_message(std::string message)
{
	_message.setup(message);
}

int	User::get_fd() const
{
	return (_fd);
}

Message	User::get_message() const
{
	return (_message);
}
