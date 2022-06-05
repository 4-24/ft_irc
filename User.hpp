#ifndef USER_HPP
# define USER_HPP

# include "Message.hpp"

class User
{
	private:
		int		_fd;
		Message	_message;

	public:
		User(int fd);
		~User();

		void		send_message(std::string message);

		int			get_fd() const;
		Message		get_message() const;
};

#endif
