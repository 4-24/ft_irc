# include "Room.hpp"

Room::Room (std::string name)
{
	_name = name;
	_topic = "";
}

Room::Room(const Room& room)
{
	_name = room._name;
	_topic = room._topic;
	_users = room._users;
}

Room&	Room::operator=(const Room &room)
{
	_name = room._name;
	_topic = room._topic;
	_users = room._users;
	return (*this);
}

Room::~Room () {}

void	Room::add_user(User *user)
{
	user->up_room_count();
	_users.push_back(user);
}

void	Room::remove_user(User &user)
{
	for (unsigned int i = 0; i < _users.size(); i++)
		if (_users[i] == &user)
		{
			_users.erase(_users.begin() + i);
			break;
		}
}

void	Room::remove_user(std::string name)
{
	for (unsigned int i = 0; i < _users.size(); i++)
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
	std::cout << "get :" << _users[0]->get_nickname() << std::endl;
	if (_users.size() > 0)
	{
		std::cout << "users: " << _users.size() << std::endl;
		for (unsigned int i = 0; i < _users.size(); i++)
		{
			std::cout << "index :" << i << "\n" << std::endl;
			std::cout << "닉네임 : " << _users[i]->get_nickname() << "\n" << std::endl;
			ss << _users[i]->get_nickname() << " ";
		}
	}
	return ss.str();
}

void	Room::send_all(std::string msg)
{
	if (_users.size() > 0)
	{
		for (unsigned int i = 0; i < _users.size(); i++)
			send(_users[i]->get_fd(), msg.c_str(), msg.size(), 0);
	}
}

void	Room::set_topic(std::string topic)
{
	_topic = topic;
}

std::string	Room::get_topic() const
{
	return _topic;
}

std::string	Room::get_name() const
{
	std::cout << "user[0] :" << _users[0]->get_nickname() << std::endl;
	std::cout << "get_name : " << _name << std::endl;
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
		for (unsigned int i = 0; i < _users.size(); i++)
			if (_users[i]->get_nickname() == name)
				return true;
	}
	return false;
}

int	Room::get_user_idx(std::string name) const
{
	if (_users.size() > 0)
	{
		for (unsigned int i = 0; i < _users.size(); i++)
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
