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

/*	config / defines	*/
# define TIMEOUT		3 * 60 * 1000	// 3min
# define PORT			4181 //debug
# define BUFFER_SIZE	512
# define SERV_NAME		"Teapot"
# define SERV_ADDR		"irc_serv.42HN.de"
# define SERV_VERS		"4.2"
# define SERV_DATE		"20.04.69"
# define NEWLINE()		std::cout << std::endl
# define NPOS			std::string::npos
# define CRLF			"\r\n"

/*	IRC-Numerics	*/
# define RPL_WELCOME	":irc_serv.42HN.de 001 " + _nick_name + " :Welcome to the Internet Relay Network " + _nick_name + "!" + _user_name + "@" + SERV_ADDR + CRLF
# define RPL_YOURHOST	":irc_serv.42HN.de 002 " + _nick_name + " :Your host is " + SERV_NAME + ", running version " + SERV_VERS + CRLF
# define RPL_CREATED	":irc_serv.42HN.de 003 " + _nick_name + " :This server was created " + SERV_DATE + CRLF

/*	classes	*/
class User
{
	private:
		User(void);

		int		initiate_handshake(std::string msg);
		int		process_handshake(void);
		void	remove_line(int time);
		int		send_welcome_reply(void);

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

/*	enumerations */
enum e_commands
{
				// CONNECTION REGISTRATION
	NICK,		// Change Nickname							[ ]	[E]RFC 2812 3.1.2
	USER,		// Set username and other options			[ ]	[E]RFC 2812 3.1.3
	OPER,		// Gain Operator Status						[ ]	[E?]RFC 2812 3.1.4
	QUIT,		// Acknowleges to client that user quit		[ ]	RFC 2812 3.1.7

				// CHANNEL OPERATIONS
	JOIN,		// Joining Channel							[ ]	[E]RFC 2812 3.2.1
	PART,		// Leaving Channel							[ ]	[E?]RFC 2812 3.2.2
	TOPIC,		// Set or change a channels description		[ ]	RFC 2812 3.2.4
	NAMES,		// List users in channel					[ ]	RFC 2812 3.2.5
	LIST,		// List channels in server					[ ]	RFC 2812 3.2.6
	INVITE,		// Invite user to channel					[ ]	RFC 2812 3.2.7
	KICK,		// Kick user from channel					[ ]	[E?]RFC 2812 3.2.8

				// SENDING MESSAGES
	PRIVMSG,	// Sending private message to a user		[ ]	[E]RFC 2812 3.3.1
	NOTICE,		// Kinda the same as PRIVMSG				[ ]	[E]RFC 2812 3.3.2
	
				// SERVER QUERIES
	INFO,		// Information about the server (help?)		[ ]	RFC 2812 3.4.10

				// MISCElLANEOUS MESSAGES
	KILL,		// Close a client-server connection			[ ]	RFC 2812 3.7.1
	PONG,		// Reply to PING message					[ ]	[E]RFC 2812 3.7.3

				// OPTIONAL FEATURES
	DIE,		// Shuts down the server					[ ]	RFC 2812 4.3
};

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
int		info(const char* info_msg); //debug
void	print_str_with_crlf(const char* s, bool print_nonprint = false); //debug

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
