#ifndef ROOM_HPP
# define ROOM_HPP

# include <string>
# include <sstream>
# include <sys/socket.h>
#include "User.hpp"

class	Room
{
	private:
		std::string			_name;
		std::string 		_topic;
		std::string 		_key;
		std::vector<User *>	_users;

	public:
		Room(std::string name);
		~Room();

		void				add_user(User &user);
		void				remove_user(std::string name);
		void				remove_user(User &user);
		void				send_all(std::string msg);
		void				set_topic(std::string topic);
		void				set_key(std::string key);


		std::string			get_name() const;
		std::string			get_topic() const;
		std::string			get_key() const;
		std::vector<User *>	get_users() const;
		std::string			get_user_list();
		bool				is_user(std::string name) const;
		bool				is_admin(std::string name) const;
};

#endif
