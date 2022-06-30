#ifndef USER_HPP
# define USER_HPP

# include <set>
# include <iostream>
# include <sys/socket.h>
# include "Message.hpp"

class User
{
	private:
		int			_fd;
		Message		_message;
		bool		_is_registered;
		bool		_is_authenticated;
		bool		_is_admin;
		std::string	_buffer;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::set<std::string> _rooms;

		time_t		_last_message_time;
		time_t		_message_timeout;

	public:
		User();
		User(int fd);
		~User();
		User(const User& user);
		User&	operator=(const User &user);

		void		setup_message();
		void		add_buffer(std::string message);
		void		clear_message();

		bool		is_registered();
		bool		is_authenticated();
		bool		is_admin();

		void		set_authenticated(bool authenticated);
		void		set_registered(bool registered);
		void		set_fd(int fd);
		void		set_nickname(std::string nickname);
		void		set_username(std::string username);
		void		set_realname(std::string realname);
		void		set_last_message_time(time_t last_message_time);
		void		set_message_timeout(time_t message_timeout);
		void		set_admin(bool admin);

		std::string	buffer() const;
		int			fd() const;
		Message		message() const;
		std::string	prefix() const;
		std::string	nickname() const;
		std::string	username() const;
		std::string	realname() const;
		std::string	fullname() const;
		time_t		last_message_time() const;
		time_t		message_timeout() const;
		bool		is_admin() const;

		std::set<std::string>& rooms(void);
		void	send_msg(std::string msg, int flag=0);
		void	send_err(std::string msg, int flag=0);
};

#endif
