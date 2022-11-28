#ifndef IRC_HPP
# define IRC_HPP

/*	includes	*/
# include <iostream>
# include <sys/socket.h>
# include <cerrno>			//errno
# include <cstring>			//strerror
# include <sys/poll.h>		//poll
# include <fcntl.h>			//fcntl
# include <vector>			//vector
# include <map>				//map

//for tests
# include <netinet/in.h>
# include <unistd.h>

//for linux
# include <cstdio>
# include <cstdlib>

/*	config	*/
# define TIMEOUT		3 * 60 * 1000	// 3min
# define PORT			4181 //debug
# define BUFFER_SIZE	512
# define SERVER_NAME	"Teapot"
# define SERVER_ADDR	"irc_serv.42HN.de"

/*	defines	*/
# define NEWLINE()	std::cout << std::endl
# define NPOS		std::string::npos

/*	classes	*/
class User
{
	private:
		User(void);

		int	initiate_handshake(const char* msg);

		const int	_fd;
		std::string	_client_msg;
		bool		_first_msg;
		std::string	_nick_name;
		std::string	_user_name;
		std::string	_real_name;
		int			_user_mode;

	public:
		User(int fd);
		~User(void);

		int			getFd(void) const;
		std::string	getClientMsg(void) const;
		int			process_msg(const char* msg);
};

/*	structs	*/
typedef struct s_serv
{
	int					listen_sd;
	char				buffer[BUFFER_SIZE];
	struct sockaddr_in	address;
	std::vector<pollfd>	fds;
	std::map<int, User>	users;
} t_serv;

/*	init.cpp	*/
int	initialization(t_serv* serv);

/*	loop.cpp	*/
int	irc_loop(t_serv* serv);

/*	utils.cpp	*/
int		error(int errno_code);
int		error(const char* error_msg);
void	ft_exit(int exit_code);
bool	is_ewouldblock(int errno_code);
void	compress_array(t_serv* serv);
int		erase_element(t_serv* serv, size_t index);
int	info(const char* info_msg); //debug

/*	error messages	*/
# define POLLEXP	"Poll time out expired"
# define REVENT		"Unexpected return event result"
# define CCLOSE		"Connection closed by client"
# define INVARGC	"Invalid argument count.\nUSAGE: ./ircserv <port> <password>"

/*	colors	*/
# define BLK "\e[0;30m"
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
# define MAG "\e[0;35m"
# define CYN "\e[0;36m"
# define WHT "\e[0;37m"
# define BBLK "\e[1;30m"
# define BRED "\e[1;31m"
# define BGRN "\e[1;32m"
# define BYEL "\e[1;33m"
# define BBLU "\e[1;34m"
# define BMAG "\e[1;35m"
# define BCYN "\e[1;36m"
# define BWHT "\e[1;37m" 
# define RESET "\e[0m"

# endif
