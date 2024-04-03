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
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];

	std::string request;
	ssize_t 	bytes_read;
	
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
				break;
		}
		request.append(buffer, bytes_read);
		if (request.substr(request.length() - 4) == "\r\n\r\n")
			break;
	}
	if (bytes_read == -1)
	{
		std::cerr << "Error reading data from client" << std::endl;
		std::cerr << "errno == " << errno << " | errno message: " << strerror(errno) << std::endl;
		return;
	}
	std::cout << "Received HTTP Request:" << std::endl;
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
	int					max_fd = server.fd;
	fd_set				read_fds;
	std::vector<int>	clients;

	FD_ZERO(&read_fds);
	FD_SET(server.fd, &read_fds);

	while (true)
	{
		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
		{
			std::cerr << "Error in select()" << std::endl;
			continue;
		}

		// Check for new connection requests
		if (FD_ISSET(server.fd, &read_fds))
		{
			int client_fd = accept(server.fd, NULL, NULL);
			if (client_fd != -1)
			{
				if (client_fd > max_fd)
					max_fd = client_fd;
				FD_SET(client_fd, &read_fds);
				clients.push_back(client_fd);
			}
		}

		// Check for data from existing connections
		for (std::vector<int>::iterator it = clients.begin(); it != clients.end();)
		{
			int client_fd = *it;
			if (FD_ISSET(client_fd, &read_fds))
			{
				handleConnection(client_fd);
				FD_CLR(client_fd, &read_fds);
				close(client_fd);
				it = clients.erase(it);
			}
			else
				++it;
		}
	}

	close(server.fd);
	return (0);
}