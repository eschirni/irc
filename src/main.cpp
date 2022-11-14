#include "irc.hpp"

bool	g_status = true;

int main(int argc, char **argv)
{
	t_serv serv;

//	if (argc != 3)
//		return error();
	init_signal_handling();
	if (initialization(&serv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (irc_loop(&serv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (clean_up(&serv) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	std::cout << "EXIT_SUCCESS" << std::endl; //debug
	return (EXIT_SUCCESS);
}