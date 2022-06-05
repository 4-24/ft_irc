# include "Message.hpp"

Message::Message(std::string message)
{
	setup(message);
}

Message::~Message() {}

bool	Message::is_end(std::string message)
{
	int	i;

	i = 0;
	while (message[i] && message[i] != '\r' && i < 510)
		i++;
	if (message[i] == '\r' && message[i + 1] == '\n')
		return true;
	else
		return false;
}

void	Message::setup(std::string message)
{
	int	i;

	i = 0;
	if (is_end(message))
	{
		if (message[i] == ':')
			parse_prefix(message, i);
		parse_command(message, i);
		parse_params(message, i);
	}
}

void	Message::parse_prefix(std::string message, int &i)
{
	i = 1;
	while (message[i] != '\r' && message[i] != ' ')
		i++;
	if (i > 1)
		_prefix.append(&message[1], i - 1);
	while (message[i] == ' ')
		i++;
}

void	Message::parse_command(std::string message, int &i)
{
	int	j = i;

	while (message[i] != '\r' && message[i] != ' ')
		i++;
	if (i > j)
		_command.append(&message[j], i - j);
}

void	Message::parse_params(std::string message, int &i)
{
	int			j;
	std::string	s;

	while (message[i] != '\r')
	{
		while (message[i] == ' ')
			i++;
		j = i;

		if (message[i] == ':') // param이 message일 경우
		{
			j++;
			while (message[i] != '\r')
				i++;
			_params.push_back(s.append(&message[j], i - j));
			return ;
		}

		while (message[i] != '\r' && message[i] != ' ')
			i++;
		if (i > j)
			_params.push_back(s.append(&message[j], i - j));
		s.clear();
	}
}

std::string					Message::get_prefix() const { return _prefix; }

std::string					Message::get_command() const { return _command; }

std::vector<std::string>	Message::get_params() const { return _params; }
