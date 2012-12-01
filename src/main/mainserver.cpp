#include "src/server/server.h"

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{
  std::cout << "main() - start!" << std::endl;

  Server server;
//  server.setPort();

  server.startServing();

  std::cout << "main() - end!" << std::endl;
}
