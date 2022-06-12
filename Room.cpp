# include "Room.hpp"

Room::Room (std::string name)
{
	_name = name;
}

Room::~Room () {}

void	Room::add_user(User &user)
{
	_users.push_back(&user);
}

void	Room::remove_user(User &user)
{
	for (size_t i = 0; i < _users.size(); i++)
		if (_users[i] == &user)
		{
			_users.erase(_users.begin() + i);
			break;
		}
}

void	Room::show_info()
{
	std::cout << "------------------------------------" << std::endl;
	std::cout << "# Room name: " << _name << std::endl;
	std::cout << "------------------------------------" << std::endl;

	std::cout << "# Users: " << std::endl;
	if (_users.size() > 0)
	{
		std::cout << "users: ";
		for (unsigned long i = 0; i < _users.size(); i++)
			std::cout << _users[i]->get_nickname() << " ";
		std::cout << std::endl;
	}
}

std::string	Room::get_name() const
{
	return (_name);
}

std::vector<User *>	Room::get_users() const
{
	return (_users);
}
