#ifndef IRC_HPP
# define IRC_HPP

/*	includes	*/
#include <iostream>
#include <sys/socket.h>
#include <cerrno>	// errno
#include <cstring>	// strerror

//for tests
#include <netinet/in.h>
#include <unistd.h>

/*	structs	*/
typedef struct s_data
{
	struct sockaddr_in	address;
} t_data;

/*	init.cpp	*/
int	initialization(t_data* data);

/*	loop.cpp	*/
int	irc_loop(t_data* data);

/*	clean.cpp	*/
int	clean_up(t_data* data);

/*	utils.cpp	*/

# endif