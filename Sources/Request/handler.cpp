# include "../../Includes/webtest.hpp"

std::string	getPageContent(std::string path)
{
	std::stringstream	ss;
	std::ifstream		file(path);

	if (file.is_open())
	{
		ss << file.rdbuf();
		file.close();
	}
	else
		std::cerr << "Could not open file" << std::endl;
	return (ss.str());
}

void servePage(int client_fd)
{
	std::ostringstream	responseStream;
	std::string			pageContent = getPageContent("Sources/Pages/index.html");

	responseStream << "HTTP/1.1 200 OK\r\n";
	responseStream << "Content-Type: text/html\r\n";
	responseStream << "Content-Length: " << pageContent.length() << "\r\n";
	responseStream << "\r\n";
	responseStream << pageContent;

	std::string	responseString = responseStream.str();
	send(client_fd, responseString.c_str(), responseString.length(), 0);
}

std::string	getRequestHeader(int client_fd)
{
	const int	BUFFER_SIZE = 1;
	char		buffer[BUFFER_SIZE];

	std::string	header = "";
	ssize_t		bytes_read;

	while (1)
	{
		bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
		if (bytes_read == 0)
			break;
		if (bytes_read == -1)
		{
			std::cerr << "Error in recv(): " << strerror(errno) << std::endl;
			exit(1); /* /!\ */
		}
		header.append(buffer, bytes_read);
		if (header.length() >= 4 && header.substr(header.length() - 4) == "\r\n\r\n")
			break;
	}
	return (header);
}

void handleConnection(int client_fd)
{
	std::cout << GREEN "Received Header:" CLEAR << std::endl;
	std::cout << getRequestHeader(client_fd) << std::endl;
	servePage(client_fd);
}
