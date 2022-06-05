# ifndef MESSAGE_HPP
#define MESSAGE_HPP

# include <string>
# include <vector>

class Message
{
	private:
		std::string					_prefix;
		std::string					_command;
		std::vector<std::string>	_params;

	public:
		Message(std::string message);
		~Message();

		void						setup(std::string message);

		std::string					get_prefix() const;
		std::string					get_command() const;
		std::vector<std::string>	get_params() const;
		void						set_prefix(std::string prefix);
		void						set_command(std::string command);
		void						set_params(std::vector<std::string> params);
};

#endif
