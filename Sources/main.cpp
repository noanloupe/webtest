#include "../Includes/webtest.hpp"

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
	{
		std::cerr << "No arg for now." << std::endl; // Config file later
		return (1);
	}

	try
	{
		/*** SERVER SETUP ***/
		s_server	server;

		server = setupServer();

		/*** CONNECTIONS ***/
		runServer(server);
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << e.what() << CLEAR << std::endl;
	}

	return (0);
}