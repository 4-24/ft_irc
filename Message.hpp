# ifndef MESSAGE_HPP
#define MESSAGE_HPP

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>

class Message
{
	private:
		std::string					_prefix;
		std::string					_command;
		std::string					_msg;
		std::vector<std::string>	_params;

	public:
		Message();
		~Message();

		void						setup(std::string message);
		bool						has_end(std::string message);
		void						add_buffer(std::string message);
		void						parse_prefix(std::string message, int &i);
		void						parse_command(std::string message, int &i);
		void						parse_params(std::string message, int &i);

		std::string					get_prefix() const;
		std::string					get_command() const;
		std::vector<std::string>	get_params() const;
};

#endif
