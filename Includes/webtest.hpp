#ifndef WEBTEST_HPP
# define WEBTEST_HPP

# include <iostream>
# include <fstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>

typedef struct s_server {
	int port;
	int fd;
	sockaddr_in sockaddr;
} t_server;

#endif