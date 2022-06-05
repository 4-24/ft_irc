# include "Message.hpp"

Message::Message(std::string message)
{
	setup(message);
}

Message::~Message() {}

void	Message::setup(std::string message)
{

}

void	Message::set_prefix(std::string prefix)
{

}
void	Message::set_command(std::string command)
{

}
void	Message::set_params(std::vector<std::string> params)
{

}

std::string					Message::get_prefix() const {}

std::string					Message::get_command() const {}

std::vector<std::string>	Message::get_params() const {}
