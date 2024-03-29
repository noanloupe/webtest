#include "../Includes/webtest.hpp"

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
	{
		std::cerr << "No arg for now." << std::endl;
		return (1);
	}

	s_server	server;

	server.port = 8090;
	server.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server.fd == -1)
	{
		std::cerr << "Error: socket" << std::endl;
		return (1);
	}
	if (fcntl(server.fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(server.fd);
		std::cerr << "Error: fcntl" << std::endl;
		return (1);
	}

	int	on = 1;
	if (setsockopt(server.fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) == -1)
	{
		close(server.fd);
		std::cerr << "Error: setsockopt" << std::endl;
		return (1);
	}

	server.sockaddr.sin_family = AF_INET;
	server.sockaddr.sin_addr.s_addr = INADDR_ANY;
	server.sockaddr.sin_port = htons(server.port);

	if (bind(server.fd, (const struct sockaddr*)&server.sockaddr, sizeof(server.sockaddr)) == -1)
	{
		close(server.fd);
		std::cerr << "Error: bind" << std::endl;
		return (1);
	}
	if (listen(server.fd, SOMAXCONN) == -1)
	{
		close(server.fd);
		std::cerr << "Error: listen" << std::endl;
		return (1);
	}
}