# include "../../Includes/webtest.hpp"

void	runServer(s_server& server)
{
	std::vector<pollfd>	fds;
	int					poll_ret;
	int					client_fd;

	std::cout << CYAN "Starting server on port " << server.port << "..." CLEAR << std::endl;
	if (bind(server.fd, (const struct sockaddr*)&server.sockaddr, sizeof(server.sockaddr)) == -1)
	{
		close(server.fd);
		std::cerr << "Error: bind" << std::endl;
		throw(tempError());
	}
	if (listen(server.fd, SOMAXCONN) == -1)
	{
		close(server.fd);
		std::cerr << "Error: listen" << std::endl;
		throw(tempError());
	}
	std::cout << CYAN "Server successfully started" CLEAR << std::endl << std::endl;

	fds.push_back({server.fd, POLLIN, 0});
	while (true)
	{
		poll_ret = poll(fds.data(), fds.size(), -1);
		if (poll_ret == -1)
		{
			std::cerr << "Error in poll(): " << strerror(errno) << std::endl;
			continue;
		}
		if (fds[0].revents & POLLIN)
		{
			client_fd = accept(server.fd, NULL, NULL);
			if (client_fd != -1)
			{
				if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
				{
					close(client_fd);
					std::cerr << "Error: fcntl" << std::endl;
					throw(tempError());
				}
				fds.push_back({client_fd, POLLIN, 0});
			}
		}

		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				handleConnection(fds[i].fd);
				close(fds[i].fd);
				fds.erase(fds.begin() + i);
				--i;
			}
		}
	}
	close(server.fd);
}