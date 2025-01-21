#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#endif

#include "quickfix/config.h"
#include "quickfix/FileStore.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/Log.h"
#include "quickfix/SessionSettings.h"
#include "qfixapp.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

void wait()
{
  std::cout << "Type Ctrl-C to quit" << std::endl;
  while(true)
  {
    FIX::process_sleep(1);
  }
}

int main( int argc, char** argv )
{
    if ( argc < 2 )
    {
        std::cout << "usage: " << argv[ 0 ]
        << " FILE." << std::endl;
        return 0;
    }
    std::string file = argv[1];

    try
    {
        qfixapp application;
        FIX::SessionSettings settings( file );
        FIX::FileStoreFactory storeFactory( settings );
        FIX::ScreenLogFactory logFactory( settings );
        std::unique_ptr<FIX::Acceptor> acceptor;
        acceptor = std::unique_ptr<FIX::Acceptor>(
            new FIX::SocketAcceptor(application, storeFactory, settings, logFactory));
        while (true) {
          acceptor->poll();
          //FIX::process_sleep(1);
          //printf("In the poll\n");
        }
        wait();
        acceptor->stop();
      return 0;
    }
    catch ( std::exception & e )
    {
      std::cout << e.what() << std::endl;
      return 1;
    }
}
