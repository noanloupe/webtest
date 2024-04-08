#include "../Includes/webtest.hpp"

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
	{
		std::cerr << "No arg for now." << std::endl;
		return (1);
	}

	/*** SERVER SETUP ***/
	s_server	server;

	server = setupServer();

	/*** CONNECTIONS ***/
	runServer(server.fd);

	return (0);
}