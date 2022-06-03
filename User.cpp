# include "User.hpp"

User::User(int fd) { _fd = fd; }

User::~User() {}

int	User::get_fd() const
{
	return (_fd);
}
