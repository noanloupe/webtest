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
	if (line == boundary && line == boundary + "--")
		return ;


	std::cout << GREEN "Started downloading files..." CLEAR << std::endl;
	while (std::getline(ss, line))
	{
		// Check if the line is the boundary indicating the end of the request
		if (line == boundary + "--")
		{
			std::cout << CYAN "Found ending boundary" CLEAR << std::endl;
			break;
		}

		// Extract file name and creating file stream
		std::ofstream	outfile;
		std::string		fileName = line.substr(line.find("filename=\"") + 10);
		fileName.erase(fileName.size() - 2);

		// Check if sending actual file and skipping to next boundary if not
		if (fileName == "")
		{
			while (std::getline(ss, line) && line != boundary)
			{
				;
			}
			continue ;
		}

		// Opening file based on fileName
		std::cout << YELLOW "Opening file: " FUSCHIA << fileName << CLEAR << std::endl;
		outfile.open("Uploads/" + fileName, std::ios::binary);
		if (!outfile.is_open())
		{
			std::cerr << "Failed to open file: " << fileName << std::endl;
			continue ;
		}

		// Skip until empty line
		while (std::getline(ss, line) && line != "\r")
		{
			;
		}

		// Read and write file data until reaching the boundary
		while (std::getline(ss, line))
		{
			if (line == boundary || line == boundary + "--")
			{
				std::cout << CYAN "Found boundary" CLEAR << std::endl;
				break;
			}
			outfile << line << std::endl;
		}

		// Closing file stream
		std::cout << YELLOW "Closing file: " FUSCHIA << fileName << CLEAR << std::endl; 
		outfile.close();
	}
	std::cout << GREEN "Finished downloading files!" CLEAR << std::endl;
}

void	handleData(s_request& request)
{
	if (access("Uploads", F_OK) != 0)
	{
		throw (tempError());
	}

	std::string	boundary = "--" + request.header["Content-Type"].substr(request.header["Content-Type"].find("boundary=") + 9) + "\r";
	
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
