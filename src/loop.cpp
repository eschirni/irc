#include "../include/irc.hpp"

static int	check_approval(t_serv* serv, User& user)
{
	if (user.getApproved() == true)
		return EXIT_SUCCESS;
	else if (user.getFirstMsg() == false)
		return EXIT_FAILURE;

	std::string msg;
	std::string buffer = static_cast<std::string>(serv->buffer);
	size_t pos = buffer.find("PASS") + static_cast<std::string>("PASS ").length();
	size_t crlf = buffer.find("\r\n");
	std::string user_password = buffer.substr(pos, crlf - pos);
	if (serv->password.compare(user_password) != 0)
	{
		msg = ERR_PASSWDMISMATCH;
		send(user.getFd(), msg.c_str(), msg.length(), 0);
		return EXIT_FAILURE;
	}

	remove_line(buffer);
	buffer.resize(BUFFER_SIZE);
	strncpy(serv->buffer, buffer.c_str(), BUFFER_SIZE);
	user.setApproved(true);
	return EXIT_SUCCESS;
}

static bool	establish_new_connection(t_serv* serv)
{
	int usr_fd = accept(serv->listen_sd, NULL, NULL);
	if (usr_fd < 0)
		return is_ewouldblock(errno);
	serv->fds.push_back(pollfd());
	serv->fds.back().fd = usr_fd;
	serv->fds.back().events = POLLIN;
	serv->users.insert(std::pair<int, User>(serv->fds.back().fd, User(usr_fd, serv)));
	return true;
}

static int	process_existing_connection(t_serv* serv, size_t index)
{
	int return_code;

	memset(serv->buffer, 0, BUFFER_SIZE);
	return_code = recv(serv->fds[index].fd, serv->buffer, BUFFER_SIZE, 0);
	if (return_code < 0 && is_ewouldblock(errno) == false)
		return (erase_element(serv, index));
	else if (return_code == 0) //connection closed by client
		return (erase_element(serv, index), error(CCLOSE));
	if (check_approval(serv, serv->users.find(serv->fds[index].fd)->second) == EXIT_FAILURE)
		return EXIT_FAILURE;
	serv->users.find(serv->fds[index].fd)->second.process_msg();
	return EXIT_SUCCESS;
}

int	irc_loop(t_serv* serv)
{
	int	return_code;
	bool status = true;

	while (status == true)
	{
		return_code = poll(&serv->fds[0], serv->fds.size(), TIMEOUT);
		if (return_code < 0)
			return(error(errno)); //for failure
		else if (return_code == 0) //for timout
			return (error(POLLEXP));
		for (int i = 0; i < (int)serv->fds.size(); i++)
		{
			/* CHECK FOR DISCONNECT OR FAILURE*/
			if (serv->fds[i].revents == 0)
				continue;
			else if (serv->fds[i].revents & POLLHUP)
			{
				erase_element(serv, i);
				continue;
			}
			else if (serv->fds[i].revents != POLLIN)
				return (error(REVENT));

			/* CHECK IF NEW CONNECTION */
			if (serv->fds[i].fd == serv->listen_sd)
				status = establish_new_connection(serv);
			else
				process_existing_connection(serv, i);
		}
	}
	return EXIT_SUCCESS;
}