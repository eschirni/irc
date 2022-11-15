#include "irc.hpp"

/*********************** CONSTRUCTION / DESTRUCTION ****************************/

User::User(void) : _fd(0) {}

User::User(int fd, char* buffer) : _fd(fd)
{
	/* code */
}

User::~User(void) {}


/**************************** GETTER / SETTER **********************************/

int	User::getFd(void) const {return _fd;}

std::string	User::getClientMsg(void) const {return _client_msg;}


/******************************** METHODS **************************************/