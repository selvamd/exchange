/**
 * \brief accept client to test tcp utility functions
 * 
 * \copyright
 */

#include <iostream>
 
#include "TcpUtils.hpp"

int main(int argc, char ** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " HostIpAddress Port" << std::endl;
        return -1;
    }

    int port = atoi(argv[2]);
    if (port <= 0)
    {
        std::cout << "Error, port must be > 0" << std::endl;
        return -1;
    }

    int rc = 0;
    int sock = 0;
    
    if ((rc = connectTCP(argv[1], port, sock)) < 0)
    {
        std::cout << "Error connecting: " << rc << std::endl;
        return -1; 
    }
    
    return 0;
}

