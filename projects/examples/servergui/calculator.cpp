#include "calculator.h"

#include <QTime>

Calculator::Calculator(Server *server)
  : ServerObserver(server),
    m_server(server)
{
}


Calculator::~Calculator()
{
}


void
Calculator::notifyReceivedData(std::vector<std::string> dataStrings, int connectionId)
{
  //  std::cout << "Calculator::notifyReceivedData() - connectionID: " << connectionId << std::endl;

  QTime timer;
  timer.start();

  try
  {
    if (dataStrings.empty())
    {
      return;
      m_server->send("received empty message!", connectionId);
    }

    if (dataStrings[0] == "add")
    {
      double sum = atof(dataStrings[1].c_str()) + atof(dataStrings[2].c_str());
      double something = std::pow(sum, 4) * cos(sum) + sin(sum);
      for (std::size_t i = 0; i < 10; ++i)
      {
        something += std::pow(something, 4) * cos(something) + sin(something);
      }
      m_server->send(std::to_string(sum), connectionId);
    }
    else
    {
      m_server->send("Hello!", connectionId);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Calculator::notifyReceivedData() - write error: " << e.what() << std::endl;
  }
//  std::cout << "Calculator::notifyReceivedData() - connectionID: " << connectionId << " time to compute and write: " << timer.elapsed()<< std::endl;
}

