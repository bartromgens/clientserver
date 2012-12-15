#ifndef ClientServerData_H
#define ClientServerData_H

#include <string>

namespace ClientServerData
{
  /** The default IP address, localhome */
  const std::string defaultIp = "127.0.0.1";
  /** The default port number */
  const unsigned short defaultPort = 2020;
  /** The default size of the char buffer in the sockets */
  const unsigned int defaultBufferSize = 2048;
  /** The separation character that separates string arguments */
  const std::string separationCharacter = "@";
  /** The default client name */
  const std::string defaultClientName = "noname";
}

#endif
