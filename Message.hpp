# ifndef MESSAGE_HPP
#define MESSAGE_HPP

# include <iostream>
# include <string>
# include <vector>

class Message
{
	private:
		std::string					_prefix;
		std::string					_command;
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

		std::string					prefix() const;
		std::string					command() const;
		std::vector<std::string>	params() const;
};

#endif
