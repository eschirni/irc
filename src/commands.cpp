#include "../include/irc.hpp"

/****************************** COMMANDS ***************************************/

int	User::info(void)
{
	std::string	msg;
	int			return_code;

	msg += ":irc_serv.42HN.de 371 \t\r\n";
	msg += ":irc_serv.42HN.de 371 ATTENTION:\r\n";
	msg += ":irc_serv.42HN.de 371 This info page is under construction and there is no guarantee for these commands to work or for the descriptions to be complete.\r\n";
	msg += ":irc_serv.42HN.de 371 \t\r\n";
	msg += ":irc_serv.42HN.de 371 nick -- Change nickname\r\n";
	msg += ":irc_serv.42HN.de 371 user -- Set username and other options\r\n";
	msg += ":irc_serv.42HN.de 371 oper -- Gain operator status\r\n";
	msg += ":irc_serv.42HN.de 371 quit -- quit the server\r\n";
	return_code = send(_fd, msg.c_str(), msg.length(), 0);
	if (return_code < 0)
		return EXIT_FAILURE;
	remove_line();
	return EXIT_SUCCESS;
}