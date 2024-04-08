# include "../../Includes/webtest.hpp"

s_server	setupServer(void)
{
	s_server	server;

	server.port = SERVERPORT;
	server.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server.fd == -1)
	{
		std::cerr << "Error: socket" << std::endl;
		exit(1); /* /!\ */
	}
	if (fcntl(server.fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(server.fd);
		std::cerr << "Error: fcntl" << std::endl;
		exit(1); /* /!\ */
	}

	int	on = true; // Has to be an int but works the same way has a bool would.
	if (setsockopt(server.fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		close(server.fd);
		std::cerr << "Error: setsockopt" << std::endl;
		exit(1); /* /!\ */
	}

	server.sockaddr.sin_family = AF_INET;
	server.sockaddr.sin_addr.s_addr = INADDR_ANY;
	server.sockaddr.sin_port = htons(server.port);

	if (bind(server.fd, (const struct sockaddr*)&server.sockaddr, sizeof(server.sockaddr)) == -1)
	{
		close(server.fd);
		std::cerr << "Error: bind" << std::endl;
		exit(1); /* /!\ */
	}
	if (listen(server.fd, SOMAXCONN) == -1)
	{
		close(server.fd);
		std::cerr << "Error: listen" << std::endl;
		exit(1); /* /!\ */
	}
	return (server);
}