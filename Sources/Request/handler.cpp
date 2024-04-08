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
	int			BUFFER_SIZE = 1;
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

std::string	replaceHexAndAmp(std::string src)
{
	std::string	str;
	int		 	hexValue;
	size_t 		length = src.length();
	
	for (size_t i = 0; i < length; ++i)
	{
		if (src[i] == '%' && i + 2 < length && isxdigit(src[i + 1]) && isxdigit(src[i + 2]))
		{
			sscanf(src.substr(i + 1, 2).c_str(), "%x", &hexValue);
			str += static_cast<char>(hexValue);
			i += 2;
		}
		else if (src[i] == '&')
			str += '\n';
		else
			str += src[i];
	}
	return (str);
}

std::string	getRequestBody(s_request& request)
{
	std::string	header = "";

	if (request.method != "POST")
		return (header);
	else if (request.header.find("Content-Length") == request.header.end())
	{
		std::cerr << "No content length in request header (400 bad request / 411 length required)" << std::endl;
		return (header);
	}

	int			BUFFER_SIZE = stoul(request.header["Content-Length"]);
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_read;

	bytes_read = recv(request.fd, buffer, BUFFER_SIZE, 0);
	// std::cout << RED "bytes read: " CLEAR << bytes_read << std::endl;
	if (bytes_read == -1)
	{
		std::cerr << "Error in recv(): " << strerror(errno) << std::endl;
		exit(1); /* /!\ */
	}
	header.append(buffer, BUFFER_SIZE);
	header = replaceHexAndAmp(header);
	return (header);
}

void	parseRequest(s_request& request)
{
	std::string			headerString = getRequestHeader(request.fd);
	std::istringstream	ss(headerString);
	std::string			line;

	std::getline(ss, line);
	std::istringstream	firstLineStream(line);
	firstLineStream >> request.method >> request.path;
	
	std::string headerKey;
	std::string headerValue;
	
	while (std::getline(ss, line) && line != "\r")
	{
		std::istringstream headerStream(line);
		std::getline(headerStream, headerKey, ':');
		std::getline(headerStream, headerValue);
		request.header[headerKey] = headerValue;
	}

	request.body = getRequestBody(request);
}

void	printRequest(s_request& request)
{
	std::cout << YELLOW "method: " CLEAR << request.method << std::endl;
	std::cout << YELLOW "path: " CLEAR << request.path << std::endl;
	for (std::map<std::string, std::string>::iterator it = request.header.begin(); it != request.header.end(); ++it)
	{
		std::cout << YELLOW << it->first << ": " CLEAR << it->second << std::endl;
	}
	std::cout << std::endl;
	if (request.body != "")
	{
		std::cout << YELLOW "body: " CLEAR << std::endl << request.body << std::endl;
		std::cout << std::endl;
	}
}

void	handleConnection(int client_fd)
{
	s_request	request;

	request.fd = client_fd;
	parseRequest(request);

	std::cout << LIME "Got " FUSCHIA << request.method << LIME " request" CLEAR << std::endl;
	printRequest(request);
	servePage(client_fd);
}
