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
		int			_room_idx;
		std::string	_buffer;
		std::string	_nickname;
		std::string	_username;

	public:
		User(int fd);
		~User();

		void		setup_message();
		void		add_buffer(std::string message);
		void		clear_message();

		bool		is_registered();
		bool		is_authenticated();

		void		set_authenticated(bool authenticated);
		void		set_registered(bool registered);

		void		set_nickname(std::string nickname);
		void		set_username(std::string username);
		void		set_room_idx(int room_idx);

		std::string	get_buffer() const;
		int			get_fd() const;
		Message		get_message() const;
		std::string	get_nickname() const;
		std::string	get_username() const;
		int			get_room_idx() const;
};

#endif
