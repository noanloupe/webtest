# include "../../Includes/webtest.hpp"

s_server	setupServer(void)
{
	s_server	server;

	server.port = SERVERPORT;
	server.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server.fd == -1)
	{
		std::cerr << "Error: socket" << std::endl;
		throw(tempError());
	}
	if (fcntl(server.fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(server.fd);
		std::cerr << "Error: fcntl" << std::endl;
		throw(tempError());
	}

	int	on = true; // Has to be an int but works the same way has a bool would.
	if (setsockopt(server.fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		close(server.fd);
		std::cerr << "Error: setsockopt" << std::endl;
		throw(tempError());
	}

	server.sockaddr.sin_family = AF_INET;
	server.sockaddr.sin_addr.s_addr = INADDR_ANY;
	server.sockaddr.sin_port = htons(server.port);

	return (server);
}