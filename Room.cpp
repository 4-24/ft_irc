#include "Room.hpp"

using std::string;
using std::set;
using std::map;

Room::Room() {}

Room::Room(const Room& u)
{
	*this = u;
}

Room::Room(string name) : _name(name) {}

Room& Room::operator=(const Room& u)
{
	_name = u._name;
	_topic = u._topic;
	_users = u._users;
	return *this;
}

Room::~Room() {}

bool Room::isin(std::string name)
{
	return _users.find(name) != _users.end();
}

bool Room::is_operator(std::string name)
{
	return _operator == name;
}

void Room::send_msg(map<string, User> &users, string except_user, string msg)
{
	for (set<string>::iterator itr = _users.begin(); itr != _users.end(); ++itr)
		if (*itr != except_user)
			users[*itr].send_msg(msg);
}

void Room::send_msg(map<string, User> &users, string msg)
{
	for (set<string>::iterator itr = _users.begin(); itr != _users.end(); ++itr)
		users[*itr].send_msg(msg);
}

void Room::join(User &u)
{
	_users.insert(u.nickname());
	u.rooms().insert(_name);
}

void Room::part(User& u)
{
	if (!isin(u.nickname()))
		return ;
	_users.erase(_users.find(u.nickname()));
	u.rooms().erase(u.rooms().find(_name));
}

void Room::set_operator(std::string name)
{
	_operator = name;
}

void Room::set_topic(std::string topic)
{
	_topic = topic;
}

set<string>& Room::users(void)
{
	return _users;
}

string Room::name(void) const
{
	return _name;
}

string Room::topic(void) const
{
	return _topic;
}

string Room::user_list(void) const
{
	string ret;
	for (set<string>::iterator itr = _users.begin(); itr != _users.end(); ++itr)
	{
		if (itr == _users.begin())
			ret += "@";
		ret += *itr + " ";
	}

	return ret;
}
