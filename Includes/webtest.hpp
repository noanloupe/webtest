#ifndef WEBTEST_HPP
# define WEBTEST_HPP

# include <iostream>
# include <fstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
# include <sstream>
# include <vector>
# include <poll.h>

# define RED		"\x1b[1m\x1b[38;2;255;0;0m"
# define LIME		"\x1b[1m\x1b[38;2;0;255;0m"
# define BLUE		"\x1b[1m\x1b[38;2;0;0;255m"
# define YELLOW		"\x1b[1m\x1b[38;2;255;255;0m"
# define FUSCHIA	"\x1b[1m\x1b[38;2;255;0;255m"
# define CYAN		"\x1b[1m\x1b[38;2;0;255;255m"
# define GREEN		"\x1b[1m\x1b[38;2;0;128;0m"
# define CLEAR		"\x1b[0m"

# define SERVERPORT 8090

typedef struct s_server {
	int port;
	int fd;
	sockaddr_in sockaddr;
} t_server;

s_server	setupServer(void);

void		runServer(int server_fd);

std::string	getPageContent(std::string path);
void servePage(int client_fd);
std::string	getRequestHeader(int client_fd);
void handleConnection(int client_fd);

#endif