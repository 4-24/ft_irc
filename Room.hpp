#ifndef ROOM_HPP
# define ROOM_HPP

# include <string>
# include <sstream>
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

		std::string			get_name() const;
		std::vector<User *>	get_users() const;
		std::string			get_user_list();
};

#endif
