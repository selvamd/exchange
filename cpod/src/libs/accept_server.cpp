/**
 * \brief accept server program to test tcp utility functions
 *
 * \copyright
 */

#include <iostream>
 
#include "TcpUtils.hpp"

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " Port" << std::endl;
        return -1;
    }

    int port = atoi(argv[1]);
    if (port <= 0)
    {
        std::cout << "Error, port must be > 0" << std::endl;
        return -1;
    }

    int rc = 0;
    int accept_sock = 0;
    
    if ((rc = InitializeAcceptSocketTCP("127.0.0.1", port, accept_sock)) < 0)
    {
        std::cout << "Error setting accept socket: " << rc << std::endl;
        return -1; 
    }
    
    while (1)
    {
        int client_socket = 0;
        sockaddr_in client_address;
        if ((rc = acceptConnectionTCP(accept_sock, client_socket, client_address)) < 0)
        {
            std::cout << "Error accepting socket: " << rc << std::endl;
            return -1; 
        }

        std::cout << "Accepted client" << std::endl;

        disconnectTCP(client_socket);
    }

    return 0;
}
