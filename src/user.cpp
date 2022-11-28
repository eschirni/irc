#include "irc.hpp"

/*********************** CONSTRUCTION / DESTRUCTION ****************************/

User::User(void) : _fd(0) {}

User::User(int fd) : _fd(fd) {}

User::~User(void) {}


/**************************** GETTER / SETTER **********************************/

int	User::getFd(void) const {return _fd;}

std::string	User::getClientMsg(void) const {return _client_msg;}


/******************************** METHODS **************************************/

int	User::process_msg(const char* msg)
{
	/* code */
	std::cout << "PROCESS_MSG_METHOD" << std::endl;
	return EXIT_SUCCESS;
}