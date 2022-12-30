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

		std::string						get_list(void);
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
		void		send_all(std::string msg, std::string self = "");
		bool		has_member(std::string name);
		std::string	getName(void);

};

#endif