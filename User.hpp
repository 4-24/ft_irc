#ifndef USER_HPP
# define USER_HPP

class User
{
	private:
		int	_fd;

	public:
		User(int fd);
		~User();

		int								get_fd() const;
};

#endif
