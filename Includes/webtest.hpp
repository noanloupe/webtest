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

# define RED "\x1b[1m\x1b[38;2;255;0;0m"
# define MB "\x1b[1m\x1b[38;2;25;25;599m"
# define GREEN "\x1b[1m\x1b[38;2;0;128;0m"
# define YELLOW "\x1b[1m\x1b[38;2;205;205;0m"
# define DV "\x1b[1m\x1b[38;2;148;0;211m"
# define CLEAR "\x1b[0m"

# define SERVERPORT 8090

typedef struct s_server {
	int port;
	int fd;
	sockaddr_in sockaddr;
} t_server;

#endif