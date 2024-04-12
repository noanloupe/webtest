# include "../../Includes/webtest.hpp"

void	handleUrlencoded(s_request& request)
{
	std::ostringstream response;
	response << "<html><head><title>Received Data</title></head><body>";
	response << "<h1>Received Data</h1>";
	response << "<p>" << request.body << "</p>";
	response << "<form action=\"/\" method=\"get\">";
	response << "<button type=\"submit\">Return to Menu</button>";
	response << "</form>";
	response << "</body></html>";

	std::ostringstream header;
	header << "HTTP/1.1 200 OK\r\n";
	header << "Content-Type: text/html\r\n";
	header << "Content-Length: " << response.str().length() << "\r\n";
	header << "\r\n";

	send(request.fd, header.str().c_str(), header.str().length(), 0);
	send(request.fd, response.str().c_str(), response.str().length(), 0);
}

void	handlePostRequest(s_request& request)
{
	if (request.header.find("Content-Type") == request.header.end())
	{
		servePage(request.fd, 400, "Bad Request", "Sources/Pages/ErrorPages/400.html");
		return ;
	}

	std::string	contentType = request.header["Content-Type"];
	std::cout << BLUE "Content Type: " CLEAR << contentType << std::endl;

	if (contentType == "application/x-www-form-urlencoded")
	{
		handleUrlencoded(request);
	}
	else if (contentType == "multipart/form-data")
	{
		//
	}
}
