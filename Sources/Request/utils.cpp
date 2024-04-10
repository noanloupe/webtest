# include "../../Includes/webtest.hpp"

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
		std::cerr << RED "Could not open file" CLEAR << std::endl;
	return (ss.str());
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