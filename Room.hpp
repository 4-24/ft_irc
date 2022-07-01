#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <map>

# include "User.hpp"

class Room
{
private:
	std::string _name;
	std::string _topic;
	std::set<std::string> _users;
	std::string	_operator;

public:
	Room();
	Room(const Room& u);
	Room(std::string name);
	Room& operator=(const Room& u);
	~Room();

	bool isin(std::string name);
	bool is_user(std::string name);
	bool is_operator(std::string name);
	void send_msg(std::map<std::string, User> &user, std::string except_user, std::string msg);
	void join(User &u);
	void part(User &u);
	void set_operator(std::string name);
	void set_topic(std::string topic);
	void send_msg(std::map<std::string, User> &users, std::string msg);
	std::set<std::string>& users(void);
	std::string	name(void) const;
	std::string	topic(void) const;
	std::string	user_list(void) const;
};

#endif
