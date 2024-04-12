# include "../../Includes/webtest.hpp"

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
			throw(tempError());
		}
		header.append(buffer, bytes_read);
		if (header.length() >= 4 && header.substr(header.length() - 4) == "\r\n\r\n")
			break;
	}
	return (header);
}

std::string	getRequestBody(s_request& request)
{
	std::string	body = "";

	if (request.header.find("Content-Length") == request.header.end())
		return (body);

	int			BUFFER_SIZE = stoul(request.header["Content-Length"]);
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_read;

	bytes_read = recv(request.fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read == -1)
	{
		std::cerr << "Error in recv(): " << strerror(errno) << std::endl;
		throw(tempError());
	}
	body.append(buffer, BUFFER_SIZE);
	body = replaceHexAndAmp(body);
	return (body);
}

bool	parseRequest(s_request& request)
{
	std::string			headerString = getRequestHeader(request.fd);
	std::istringstream	ss(headerString);
	std::string			line;

	std::getline(ss, line);
	std::istringstream	firstLineStream(line);
	firstLineStream >> request.method >> request.path;
	if (request.path.back() == '?')	// Remove '?' in case there is no query parameters
		request.path.erase(request.path.size() -1);
	
	std::string headerKey;
	std::string headerValue;
	
	while (std::getline(ss, line) && line != "\r")
	{
		std::istringstream headerStream(line);
		std::getline(headerStream, headerKey, ':');
		std::getline(headerStream, headerValue);
		request.header[headerKey] = headerValue.substr(1, headerValue.size() - 2);
	}

	request.body = "";
	if (request.method == "POST")
		request.body = getRequestBody(request);
	if (request.body == "")
		return (1);
	return (0);
}