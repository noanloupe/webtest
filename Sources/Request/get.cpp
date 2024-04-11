# include "../../Includes/webtest.hpp"

void	handleGetRequest(s_request& request)
{
	if (request.path == "/")
		request.path += "index.html";
	request.path = "Sources/Pages" + request.path;
	if (access(request.path.c_str(), F_OK) == -1)
	{
		servePage(request.fd, 404, "Not Found", "Sources/Pages/ErrorPages/404.html");
		return ;
	}
	servePage(request.fd, 200, "OK", request.path);
}