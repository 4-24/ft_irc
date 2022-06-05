#ifndef USER_HPP
# define USER_HPP

# include "Message.hpp"

class User
{
	private:
		int			_fd;
		Message		_message;
		bool		_is_registered;
		bool		_is_authenticated;
		std::string	_nickname;
		std::string	_username;

	public:
		User(int fd);
		~User();

		void		add_message(std::string message);

		bool		is_registered();
		bool		is_authenticated();

		void		set_authenticated(bool authenticated);
		void		set_registered(bool registered);
		int			get_fd() const;
		Message		get_message() const;
};

#endif
