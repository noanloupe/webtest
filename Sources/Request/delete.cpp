# include "../../Includes/webtest.hpp"

void	handleDeleteRequest(s_request& request)
{
	if (request.path == "/") // Avoids deleting Uploads directory in case it is empty
	{
		return ;
	}

	request.path = "Sources/Uploads" + request.path;
	if (access(request.path.c_str(), F_OK) == -1)
	{
		servePage(request.fd, 404, "Not Found", "Sources/Pages/ErrorPages/404.html");
		return ;
	}
	if (std::remove((request.path).c_str()))
	{
		std::cerr << RED "Could not delete file " BLUE << request.path << CLEAR << std::endl;
		return ;
	}
	servePage(request.fd, 200, "OK", "sources/Pages/index.html");
}