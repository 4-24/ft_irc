# include "Room.hpp"

Room::Room (std::string name)
{
	_name = name;
	_topic = "";
	_key = "";
}

Room::~Room () {}

void	Room::add_user(User &user)
{
	user.up_room_count();
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
			_users[i]->down_room_count();
			_users.erase(_users.begin() + i);
			break;
		}
}

std::string	Room::get_user_list()
{
	std::stringstream ss;
	ss.str("");

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

void	Room::set_topic(std::string topic)
{
	_topic = topic;
}

void	Room::set_key(std::string key)
{
	_key = key;
}

std::string	Room::get_topic() const
{
	return _topic;
}

std::string	Room::get_key() const
{
	return _key;
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

int	Room::get_user_idx(std::string name) const
{
	if (_users.size() > 0)
	{
		for (unsigned long i = 0; i < _users.size(); i++)
			if (_users[i]->get_nickname() == name)
				return i;
	}
	return -1;
}

bool	Room::is_admin(std::string name) const
{
	if (_users.size() > 0)
		if (_users[0]->get_nickname() == name)
			return true;
	return false;
}
