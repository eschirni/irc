#include "../include/irc.hpp"

int main(int argc, char** argv)
{
	t_serv serv;

	if (argc != 3)
		return error(INVARGC);
	if (initialization(&serv, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (irc_loop(&serv) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	std::cout << "EXIT_SUCCESS" << std::endl; //debug
	return (EXIT_SUCCESS);
}