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

void	downloadFiles(s_request& request, std::string boundary)
{
	std::istringstream 	ss(request.body);
	std::string 		line;


	std::getline(ss, line);
	if (line == boundary + "--")
		return ;
	
	while (std::getline(ss, line))
	{
		// Check if the line is the boundary indicating the end of the request
		if (line == boundary + "--")
			break;

		std::string			fileName = line.substr(line.find("filename=") + 9, line.size() - 2); // Extract file name
		std::ofstream		outfile;

		outfile.open("Uploads/" + fileName, std::ios::binary);
		if (!outfile.is_open())
		{
			std::cerr << "Failed to open file: " << fileName << std::endl;
			continue ;
		}

		// Skip until empty line
		while (std::getline(ss, line) && line != "\r");

		// Read and write file data until the boundary
		while (std::getline(ss, line) && line != boundary)
		{
			std::cout << RED "line: " << line << ", diff: " << line.compare(boundary) << ", size: " << line.size() << CLEAR << std::endl;
			outfile << line << std::endl;
		}

		outfile.close();
	}
}

void	handleData(s_request& request)
{
	if (access("Uploads", F_OK) != 0)
	{
		throw (tempError());
	}

	std::string	boundary = request.header["Content-Type"].substr(request.header["Content-Type"].find("boundary=") + 9);
	// boundary = "--" + boundary;
	std::cout << GREEN "Boundary: " FUSCHIA << boundary << ", size: " << boundary.size() << CLEAR << std::endl;
	
	downloadFiles(request, boundary);
	servePage(request.fd, 200, "OK", "Sources/Pages/Index.html");
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
	else if (contentType.substr(0, 19) == "multipart/form-data")
	{
		handleData(request);
	}
}
