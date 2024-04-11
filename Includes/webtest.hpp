#ifndef WEBTEST_HPP
# define WEBTEST_HPP

# include <iostream>
# include <fstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
# include <sstream>
# include <map>
# include <vector>
# include <poll.h>
# include <cstdio>

# define RED		"\x1b[1m\x1b[38;2;255;0;0m"
# define LIME		"\x1b[1m\x1b[38;2;0;255;0m"
# define BLUE		"\x1b[1m\x1b[38;2;0;0;255m"
# define YELLOW		"\x1b[1m\x1b[38;2;255;255;0m"
# define FUSCHIA	"\x1b[1m\x1b[38;2;255;0;255m"
# define CYAN		"\x1b[1m\x1b[38;2;0;255;255m"
# define GREEN		"\x1b[1m\x1b[38;2;0;128;0m"
# define CLEAR		"\x1b[0m"

# define SERVERPORT 8090

typedef struct s_server
{
	int port;
	int fd;
	sockaddr_in sockaddr;
}	t_server;

typedef struct s_request 
{
	int									fd;
	std::string							method;
	std::string							path;
	std::map<std::string, std::string>	header;
	std::string							body;
}	t_request;

/*	SERVER	*/
void		runServer(s_server& server);
s_server	setupServer(void);

/*	REQUEST	*/
std::string	getPageContent(std::string path);
std::string	getRequestHeader(int client_fd);
void		handleConnection(int client_fd);
void		handleDeleteRequest(s_request& request);
void		handleGetRequest(s_request& request);
bool		parseRequest(s_request& request);
void		printRequest(s_request& request);
std::string	replaceHexAndAmp(std::string src);
void		servePage(int client_fd, int code, std::string value, std::string path);

/* EXCEPTIONS */

class tempError : public std::exception
{
	public:
		const char*	what(void) const throw()
		{
			return("This is a temp throw (an error occured thought)");
		}
};

#endif