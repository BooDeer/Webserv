#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>





#define PORT 8080





int main(int ac, char **av)
{

	int	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		perror("Socket creation failed.");
		return 1;
	}
	sockaddr_in	address;
	memset((char *)&address, 0, sizeof(address));
	// address.sa_family = AF_INET;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
	if (bind(socket_fd, (sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Socker binding failed.  ");
		return 2;
	}

	return (0);
}