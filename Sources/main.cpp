#include "../Includes/webtest.hpp"

const std::string PAGE_CONTENT = "<html><body><h1>Hello, World!</h1></body></html>";

void servePage(int client_fd) {
	// Send HTTP response header
	std::ostringstream response;
	response << "HTTP/1.1 200 OK\r\n";
	response << "Content-Type: text/html\r\n";
	response << "Content-Length: " << PAGE_CONTENT.length() << "\r\n";
	response << "\r\n"; // End of headers
	response << PAGE_CONTENT;

	// Send response to client
	std::string responseStr = response.str();
	send(client_fd, responseStr.c_str(), responseStr.length(), 0);
}

void printHTTPRequest(int client_fd)
{
	const int	BUFFER_SIZE = 1024;
	char		buffer[BUFFER_SIZE];

	std::string	request;
	ssize_t		bytes_read;

	while (1)
	{
		bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
		if (bytes_read == 0)
			break;
		if (bytes_read == -1)
		{
			if (errno == 35)
				continue;
			else
			{
				std::cerr << "Error in recv(): " << strerror(errno) << std::endl;
				return;
			}
		}
		request.append(buffer, bytes_read);
		if (request.length() >= 4 && request.substr(request.length() - 4) == "\r\n\r\n")
			break;
	}
	std::cout << GREEN "Received HTTP Request:" CLEAR << std::endl;
	std::cout << request << std::endl;
}

void handleConnection(int client_fd)
{
	printHTTPRequest(client_fd);
	servePage(client_fd);
}

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

	server.port = SERVERPORT;
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

	int	on = true; // Has to be an int but works the same way has a bool would.
	if (setsockopt(server.fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
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

	/*** CONNECTIONS ***/
	std::vector<pollfd>	fds;
	int					poll_ret;
	int					client_fd;

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
				fds.push_back({client_fd, POLLIN, 0});
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
	return (0);
}