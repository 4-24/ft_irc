#ifndef ROOM_HPP
# define ROOM_HPP

# include <string>
# include "User.hpp"

class	Room
{
	private:
		std::string			_name;
		std::vector<User *>	_users;

	public:
		Room(std::string name);
		~Room();

		void				add_user(User &user);
		void				remove_user(User &user);
		void				show_users();

		std::string			get_name() const;
		std::vector<User *>	get_users() const;
};

#endif
