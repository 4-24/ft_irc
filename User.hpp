#ifndef USER_HPP
# define USER_HPP

# include "Message.hpp"

class User
{
	private:
		int			_fd;
		int			_room_count;
		Message		_message;
		bool		_is_registered;
		bool		_is_authenticated;
		bool		_is_admin;
		std::string	_buffer;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;

		time_t		_last_message_time;
		time_t		_message_timeout;

	public:
		User(int fd);
		~User();
		User(const User& user);
		User&	operator=(const User &user);

		void		setup_message();
		void		add_buffer(std::string message);
		void		clear_message();
		void		up_room_count();
		void		down_room_count();

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

		std::string	get_buffer() const;
		int			get_fd() const;
		int			get_room_count() const;
		Message		get_message() const;
		std::string	get_prefix() const;
		std::string	get_nickname() const;
		std::string	get_username() const;
		std::string	get_realname() const;
		time_t		get_last_message_time() const;
		time_t		get_message_timeout() const;
		bool		is_admin() const;
		std::string	prefix() const;
};

#endif
