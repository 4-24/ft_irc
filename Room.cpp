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

void	Room::remove_user(std::string name)
{
	for (size_t i = 0; i < _users.size(); i++)
		if (_users[i]->get_nickname() == name)
		{
			_users.erase(_users.begin() + i);
			break;
		}
}

std::string	Room::get_user_list()
{
	std::stringstream ss;

	if (_users.size() > 0)
	{
		for (unsigned long i = 0; i < _users.size(); i++)
			ss << _users[i]->get_nickname() << " ";
	}
	return ss.str();
}

void	Room::send_all(std::string msg)
{
	if (_users.size() > 0)
	{
		for (unsigned long i = 0; i < _users.size(); i++)
			send(_users[i]->get_fd(), msg.c_str(), msg.size(), 0);
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

bool Room::is_user(std::string name) const
{
	if (_users.size() > 0)
	{
		for (unsigned long i = 0; i < _users.size(); i++)
			if (_users[i]->get_nickname() == name)
				return true;
	}
	return false;
}
