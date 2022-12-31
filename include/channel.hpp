#ifndef CHANNEL_HPP
# define CHANNEL_HPP

/******************************** INCLUDES *************************************/
# pragma once
# include "user.hpp"
# include <vector>


/******************************* CHANNEL-CLASS ************************************/

class Channel
{
	private:
		/********************** PIRVATE METHODS ************************/

		void							print_list(User *usr, bool all = false);
		std::vector<User *>::iterator	get_member(std::string nick);
		std::vector<User *>::iterator	get_op(std::string nick);

		/********************** DATA-MEMBERS ***************************/

		std::vector<User *>	_ops;
		std::vector<User *>	_members;
		std::string			_name;
		std::string			_topic;

	public:
		/********************** CONSTRUCTION ***************************/

		Channel(std::string name, User *creator);

		/********************** PUBLIC METHODS *************************/
		void		join(User *usr);
		void		topic(User *usr, std::string topic);
		void		names(User *usr);
		void		part(User *usr, std::string leave_msg);
		void		op(User *usr, std::string mode, std::string name);
		void		kick(User *usr, std::string name, std::string kick_msg);
		void		send_all(std::string msg, std::string self = "");
		bool		has_member(std::string name);
		std::string	getName(void);

};

#endif