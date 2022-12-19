#ifndef USER_HPP
# define USER_HPP

/******************************** INCLUDES *************************************/

# include <string>
# include <map>


/******************************* USER-CLASS ************************************/

typedef struct s_serv t_serv;
class User
{
	typedef typename std::map<int, User>::iterator mapite_t;

	private:
		int			initiate_handshake(std::string msg);
		int			process_handshake(void);
		void		remove_line(int time = 1);
		int			send_welcome_reply(void);
		int			get_current_command(void);
		bool		check_nickname(std::string nick);
		int			info(void);
		mapite_t	get_user(std::string nick);
		void		send_all(std::string msg);
		/* cmds */
		void 		oper(std::string nick, std::string pwd);
		void		nick(const std::string nick);
		void		ping(std::string msg);

		const int	_fd;
		t_serv		*_serv;
		bool		_first_msg;
		bool		_approved;
		bool		_is_oper; //idk if server op is okay
		std::string	_client_msg;
		std::string	_nick_name;
		std::string	_user_name;
		std::string	_real_name;
		int			_user_mode;

	public:
		User(int fd, t_serv *serv);
		~User(void);

		std::string	getClientMsg(void) const;
		bool		getApproved(void) const;
		void		setApproved(bool approval);
		int			process_msg(void);
		bool		getFirstMsg(void) const;
		int			getFd(void) const;
		std::string	getNickName(void) const;
};


/********************************* ENDIF ***************************************/

#endif