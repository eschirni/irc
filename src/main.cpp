#include "irc.hpp"

static void clean_exit(int status, void* arg)
{
	t_serv*	serv = static_cast<t_serv*>(arg);
	std::cout << "clean exit called with status:\t" << status << std::endl;
	clean_up(serv);
}

int main(int argc, char **argv)
{
	t_serv serv;

//	if (argc != 3)
//		return error();
	init_signal_handling();
	if (on_exit(&clean_exit, &serv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (initialization(&serv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (irc_loop(&serv) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	std::cout << "EXIT_SUCCESS" << std::endl; //debug
	return (EXIT_SUCCESS);
}