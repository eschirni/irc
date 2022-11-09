#include "irc.hpp"

int main(int argc, char **argv)
{
	t_data				*data = 0;

	// TODO if argc
	if (initialization(data) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (irc_loop(data) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (clean_up(data) == EXIT_FAILURE)
		return EXIT_FAILURE;
	error(errno);
	error("TEST");

	std::cout << "EXIT_SUCCESS" << std::endl;
	return EXIT_SUCCESS;
}