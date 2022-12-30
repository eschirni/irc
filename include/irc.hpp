#ifndef IRC_HPP
# define IRC_HPP

/******************************** INCLUDES *************************************/

# include <iostream>		// cout, cerr, endl
# include <cerrno>			// errno, EWOULDBLOCK
# include <cstring>			// memset, strncpy, strerror
# include <sys/poll.h>		// poll, POLLIN, POLLHUP
# include <fcntl.h>			// fcntl, F_SETFL, O_NONBLOCK
# include <vector>			// vector
# include <map>				// map
# include <unistd.h>		// close
# include <cstdlib>			// strtod, atoi, exit, EXIT_SUCCESS, EXIT_FAILURE
# include "user.hpp"		// user class
# include "channel.hpp"		// channel class
# include <netinet/in.h>	// socket, send, recv, accept, htons,
							// sockaddr_in type, SOCK_STREAM, AF_INET


/********************************* CONFIG **************************************/

# define TIMEOUT		180000
# define SERV_NAME		"Teapot"
# define SERV_ADDR		"irc_serv.42HN.de"
# define SERV_VERS		"4.2"
# define SERV_DATE		"20.04.69"
# define PORT_MIN		1024
# define PORT_MAX		65535
# define PASSW_MIN_LEN	8
# define PASSW_MAX_LEN	1024


/***************************** MISC-DEFINES ************************************/

# define BUFFER_SIZE	512
# define NEWLINE()		std::cout << std::endl
# define NPOS			std::string::npos
# define CRLF			"\r\n"


/***************************** IRC-NUMERICS ************************************/

# define RPL_WELCOME		":irc_serv.42HN.de 001 " + _nick_name + " :Welcome to the Internet Relay Network " + _nick_name + "!" + _user_name + "@" + SERV_ADDR + CRLF
# define RPL_YOURHOST		":irc_serv.42HN.de 002 " + _nick_name + " :Your host is " + SERV_NAME + ", running version " + SERV_VERS + CRLF
# define RPL_CREATED		":irc_serv.42HN.de 003 " + _nick_name + " :This server was created " + SERV_DATE + CRLF
# define RPL_UMODEIS		":irc_serv.42HN.de 221 "
# define RPL_LUSERUNKNOWN	":irc_serv.42HN.de 253 0 :Unknown users\r\n"
# define RPL_AWAY			"301 RPL_AWAY "
# define RPL_UNAWAY			"305 RPL_UNAWAY :You are no longer marked as being away\r\n"
# define RPL_NOWAWAY		"306 RPL_NOWAWAY :You have been marked as being away\r\n"
# define RPL_BADCHANPASS	":irc_serv.42HN.de 339 Wrong Password\r\n"
# define RPL_KILLDONE		":irc_serv.42HN.de 361 User was kicked\r\n"
# define RPL_YOUREOPER		":irc_serv.42HN.de 381 You are oper\r\n"
# define ERR_NOSUCHNICK		":irc_serv.42HN.de 401 "
# define ERR_CANNOTSENDTOCH	":irc_serv.42HN.de 404 "
# define ERR_PASSWDMISMATCH	":irc_serv.42HN.de 464 Password incorrect.\r\n"
# define ERR_UNKNOWNCOMMAND	":irc_serv.42HN.de 421 "
# define ERR_NICKNAMEINUSE	":irc_serv.42HN.de 433 "
# define ERR_NEEDMOREPARAMS	":irc_serv.42HN.de 461 MODE :No mode specified\r\n"
# define ERR_BADCHANMASK	":irc_serv.42HN.de 476 "
# define ERR_NOPRIVILEGES	":irc_serv.42HN.de 481 " + _nick_name + " :Not an oper\r\n"
# define ERR_UMODEUNKNOWN	":irc_serv.42HN.de 501 "
# define ERR_USERSDONTMATCH	":irc_serv.42HN.de 502 "


/***************************** SERVER-STRUCT ***********************************/

typedef struct s_serv
{
	int						listen_sd;
	char					buffer[BUFFER_SIZE];
	struct sockaddr_in		address;
	std::vector<pollfd>		fds;
	std::vector<Channel>	channels;
	std::map<int, User>		users;
	std::string				password;
} t_serv;


/***************************** COMMANDS-ENUM ***********************************/

enum e_commands
{
	NICK,		// Change Nickname								RFC 2812 3.1.2
	OPER,		// Gain Operator Status							RFC 2812 3.1.4
	JOIN,		// Joining Channel								RFC 2812 3.2.1
	PART,		// Leaving Channel								RFC 2812 3.2.2
	KICK,		// Kick user from channel						RFC 2812 3.2.8
	PRIVMSG,	// Sending private message to a user			RFC 2812 3.3.1
	NOTICE,		// Kinda the same as PRIVMSG					RFC 2812 3.3.2
	PING,		// Reply to PING message						RFC 2812 3.7.3

	MODE,		// Sets user or channel modes					RFC 2812 3.1.5
	QUIT,		// Acknowleges to client that user quit			RFC 2812 3.1.7
	TOPIC,		// Set or change a channels description			RFC 2812 3.2.4
	NAMES,		// List users in channel						RFC 2812 3.2.5
	LIST,		// List channels in server						RFC 2812 3.2.6
	INVITE,		// Invite user to channel						RFC 2812 3.2.7
	LUSERS,		// Displays information about Server & Clients  RFC 2812 3.4.2
	INFO,		// Information about the server (help?)			RFC 2812 3.4.10
	KILL,		// Close a client-server connection				RFC 2812 3.7.1
	AWAY,		// Sets the User on away						RFC 2812 4.1
	DIE			// Shuts down the server						RFC 2812 4.3
};


/************************** FUNCTION-PROTOTYPES ********************************/

/* init.cpp */
int		initialization(t_serv* serv, char** argv);

/* loop.cpp */
int		irc_loop(t_serv* serv);

/* utils.cpp */
int		error(int errno_code);
int		error(const char* error_msg);
void	ft_exit(int exit_code);
bool	is_ewouldblock(int errno_code);
void	compress_array(t_serv* serv);
int		erase_element(t_serv* serv, size_t index);
int		info(const char* info_msg); //debug
void	print_str_with_crlf(const char* s, bool print_nonprint = false); //debug
void	remove_line(std::string& string, int iterations = 1);


/**************************** ERROR-MESSAGES ***********************************/

# define POLLEXP		"Poll time out expired"
# define REVENT			"Unexpected return event result"
# define CCLOSE			"Connection closed by client"
# define INVARGC		"Invalid argument count.\nUSAGE: ./ircserv <port> <password>"
# define INVPORT		"Invalid port. Port should only contain digits and be in the range of 1024 to 65535."
# define PASSTOSMALL	"Invalid password. Password should be at least 8 characters big."
# define PASSTOBIG		"Invalid password. Password to big."
# define PASSNOCHAR		"Invalid password. Password has to have at least one letter."
# define PASSNOINT		"Invalid password. Password has to have at least one digit."
# define PASSNOFT		"Invalid password. Password does not contain the sequence '42'."
# define PASSNOHN		"Invalid password. Password does not contain the sequence 'Heilbronn'."


/***************************** COLOR-DEFINES ***********************************/

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


/********************************* ENDIF ***************************************/

# endif
