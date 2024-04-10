# include "../../Includes/webtest.hpp"

void	servePage(int client_fd, int code, std::string value, std::string path)
{
	std::ostringstream	responseStream;
	std::string			pageContent = getPageContent(path);

	responseStream << "HTTP/1.1 " << code << " " << value << "\r\n";
	responseStream << "Content-Type: text/html\r\n";
	responseStream << "Content-Length: " << pageContent.length() << "\r\n";
	responseStream << "Server: Webtest\r\n";
	responseStream << "\r\n";
	responseStream << pageContent;

	std::string	responseString = responseStream.str();
	send(client_fd, responseString.c_str(), responseString.length(), 0);
}