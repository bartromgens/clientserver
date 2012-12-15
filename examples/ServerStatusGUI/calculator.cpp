#include "calculator.h"

Calculator::Calculator(Server *server)
  : ServerObserver(),
    m_server(server)
{
  m_server->registerObserver(this);
}


Calculator::~Calculator()
{
}


void
Calculator::notifyReceivedData(std::vector<std::string> dataStrings, int connectionId)
{
  //  std::cout << "Calculator::notifyReceivedData() - connectionID: " << connectionId << std::endl;

  try
  {
    if (dataStrings.empty())
    {
      return;
      m_server->write("received empty message!", connectionId);
    }

    if (dataStrings[0] == "add")
    {
      double sum = atof(dataStrings[1].c_str()) + atof(dataStrings[2].c_str());
      m_server->write(std::to_string(sum), connectionId);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Calculator::notifyReceivedData() - write error: " << e.what() << std::endl;
  }
}

