# include "../../Includes/webtest.hpp"

void	handleConnection(int client_fd)
{
	s_request	request;

	request.fd = client_fd;
	
	if (parseRequest(request))
	{
		if (request.method == "POST" && request.header.find("Content-Length") == request.header.end())	// We check that POST method has a content length and not just an empty body
		{
			servePage(client_fd, 411, "Length Required", "Sources/Pages/ErrorPages/411.html");
			return ;
		}
		else	// We check that any other method has no body (if i'm right, might cause issue if it has none and an other request comes on the same non-blocking socket but very unlikely)
		{
			char	buffer[1];
			int		ret = recv(request.fd, buffer, 1, 0);
			if (ret != -1)
			{
				servePage(client_fd, 400, "Bad Request", "Sources/Pages/ErrorPages/400.html");
				return ;
			}
		}
	}

	std::cout << LIME "Got " FUSCHIA << request.method << LIME " request" CLEAR << std::endl;
	printRequest(request);

	if (request.method == "GET")
	{
		servePage(client_fd, 200, "OK", "sources/Pages/index.html");
	}
	else if (request.method == "POST")
	{
		servePage(client_fd, 200, "OK", "sources/Pages/index.html");
	}
	else if (request.method == "DELETE")
	{
		servePage(client_fd, 200, "OK", "sources/Pages/index.html");
	}
	else
	{
		servePage(client_fd, 405, "Method Not Allowed", "Sources/Pages/ErrorPages/405.html");
	}
}
