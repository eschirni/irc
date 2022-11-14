#include "irc.hpp"

static void	signal_handler(int signum)
{
	g_status = false;
	switch (signum)
	{
	case SIGINT:
		std::cout << BMAG << "SIGINT" << RESET << " received." << std::endl;
		break;

	case SIGQUIT:
		std::cout << BMAG << "SIGQUIT" << RESET << " received." << std::endl;
		break;
	
	case SIGTSTP:
		std::cout << BMAG << "SIGTSTP" << RESET << " received." << std::endl;
	
	default:
		break;
	}
}

void	init_signal_handling(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGTSTP, signal_handler);
}
