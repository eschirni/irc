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
		/********************** PIRVATE METHODS ************************/

		void		remove_line(int time = 1);
		void		send_all(std::string msg);
		bool		check_nickname(std::string nick);
		int			initiate_handshake(std::string msg);
		int			process_handshake(void);
		int			send_welcome_reply(void);
		int			get_current_command(void);
		mapite_t	get_user(std::string nick);


		/************************ COMMANDS *****************************/

		int			info(void);
		void 		oper(std::string nick, std::string pwd);
		void		nick(const std::string nick);
		void		ping(std::string msg);


		/********************** DATA-MEMBERS ***************************/

		const int	_fd;
		t_serv		*_serv;
		bool		_first_msg;
		bool		_approved;
		bool		_is_oper;
		int			_user_mode;
		std::string	_client_msg;
		std::string	_nick_name;
		std::string	_user_name;
		std::string	_real_name;

	public:
		/********************** CONSTRUCTION ***************************/

		User(int fd, t_serv *serv);
		~User(void);


		/********************** PUBLIC METHODS *************************/

		std::string	getClientMsg(void) const;
		std::string	getNickName(void) const;
		bool		getApproved(void) const;
		bool		getFirstMsg(void) const;
		int			getFd(void) const;
		void		setApproved(bool approval);
		int			process_msg(void);
};


/********************************* ENDIF ***************************************/

#endif